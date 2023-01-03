#include <GLBase/GLBase.hpp>
#include <memory>
#include <iostream>
#include "defaultShaders.h"
#include <imgui/imgui.h>
#include <numeric>
#include <errno.h>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <cmath>
#include <UserSettings.hpp>

class GLExplorer: public GLBase::Application
{
private:
    GLBase::VertexBuffer _vbo;
    GLBase::IndexBuffer _ibo;
    GLBase::VertexArray _vao;
    GLBase::Shader _shader;
    std::string _filePath;
    std::vector<UserParam> _shaderSettings;

    std::chrono::steady_clock::time_point _start;
    float _scale = 1.0f;
    int _mouseSensitivity = 2;
    bool _fileErrorPopup = false;
    struct { float x, y, z, w; } _offset = {};
    struct { float x, y; } _mouseMotion;
public:
    GLExplorer(): GLBase::Application(500, 500, "GLSL Explorer", true, true),
        _vbo(nullptr, 3*sizeof(GLBase::Vector3), GL_STATIC_DRAW),
        _ibo(nullptr, 6, GL_STATIC_DRAW),
        _vao(), _shader(vertexShader, defaultFragShader)
    {
        _start = std::chrono::steady_clock::now();
        _vao.addBuffer(_vbo, GLBase::VertexLayout<1>(
            GLBase::VertexLayoutElement::create<float>(3)
        ));
        static constexpr uint32_t I[] = {0, 1, 2, 2, 3, 0};
        _ibo.bind();
        _ibo.setIndexData(I, 6, GL_STATIC_DRAW);
        static constexpr GLBase::Vector3 V[] = {
            {-1, -1, 0},
            {-1, 1, 0},
            {1, 1, 0},
            {1, -1, 0},
        };
        _vbo.setVertexData(V, sizeof(V), GL_STATIC_DRAW);
        // _shader.setSource(vShader, defaultFragShader);
        _vao.unbind();
        _vbo.unbind();
        _ibo.unbind();
    }
    void userInputs(std::pair<int, int> winSize, std::chrono::duration<double> delta){
        if(ImGui::GetIO().WantCaptureMouse) return;
        static auto mouseOld = getMousePos();
        auto mouseNew = getMousePos();
        _mouseMotion = {mouseNew.x-mouseOld.x, mouseOld.y-mouseNew.y}; // y axis is inverted
        mouseOld = mouseNew;
        if(glfwGetMouseButton(_window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS){
            float speed = _mouseSensitivity * _scale;
            if(glfwGetKey(_window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS){                
                _scale *= expf(-_mouseSensitivity*_mouseMotion.x / winSize.first);
            }
            else{
                _offset.x -= _mouseMotion.x * speed / float(winSize.first);
                _offset.y -= _mouseMotion.y * speed / float(winSize.second);
            }
        }
    }
    virtual void update(std::chrono::duration<double> delta) override {
        if(!_shader.isValid()){
            _shader = GLBase::Shader(vertexShader, errorFragShader);
        }
        auto size = windowSize();
        userInputs(size, delta);
        float ratio = float(size.first) / float(size.second);
        _shader.bind();
        std::chrono::duration<float> time = std::chrono::steady_clock::now() - _start;
        _shader.setUniform("uRatio", ratio);
        _shader.setUniform("time", time.count());
        _shader.setUniform("uScale", _scale);
        _shader.setUniform("uOffset", _offset.x, _offset.y, _offset.z, _offset.w);
        updateUI();
    }
    struct{float x, y;} getMousePos(){
        double x, y;
        glfwGetCursorPos(_window, &x, &y);
        return {float(x), float(y)};
    }
    static int textSizeCallback(ImGuiInputTextCallbackData* data){
        std::string* p = (std::string*)data->UserData;
        if (data->EventFlag == ImGuiInputTextFlags_CallbackResize)
        {
            IM_ASSERT(data->Buf == p->c_str());
            p->resize(data->BufSize);
            data->Buf = p->data();
        }
        return 0;
    }
    static void stringInput(const char *label, std::string& out){
        ImGui::InputText(label, out.data(), out.capacity(), 
            ImGuiInputTextFlags_CallbackResize, textSizeCallback, (void*)&out);
    }
    void dispUserSettingValue(UserParam& val)
    {
        switch (val.type)
        {
        case UserParamType_t::f1:
            ImGui::DragFloat(val.name.c_str(), val.value.f, 1.0f, 
                std::numeric_limits<float>::lowest(),
                std::numeric_limits<float>::max());
            _shader.setUniform(val.name, val.value.f[0]);
            break;
        case UserParamType_t::f2:
            ImGui::DragFloat2(val.name.c_str(), val.value.f, 1.0f, 
                std::numeric_limits<float>::lowest(),
                std::numeric_limits<float>::max());
            _shader.setUniform(val.name, val.value.f[0], val.value.f[1]);
            break;
        case UserParamType_t::f3:
            ImGui::DragFloat3(val.name.c_str(), val.value.f, 1.0f, 
                std::numeric_limits<float>::lowest(),
                std::numeric_limits<float>::max());
            _shader.setUniform(val.name, val.value.f[0], val.value.f[1], val.value.f[2]);
            break;
        case UserParamType_t::f4:
            ImGui::DragFloat4(val.name.c_str(), val.value.f, 1.0f, 
                std::numeric_limits<float>::lowest(),
                std::numeric_limits<float>::max());
            _shader.setUniform(val.name, val.value.f[0], val.value.f[1], val.value.f[2], val.value.f[3]);
            break;
        default:
            break;
        }
    }
    void userSettingsPanel(){
        for(auto& p: _shaderSettings)
            dispUserSettingValue(p);
    }
    void updateUI() {
        ImGui::Begin("Inspector");
        ImGui::Text("%f", ImGui::GetIO().Framerate);
        stringInput("Shader file", _filePath);
        if(ImGui::Button("Load")){
            _fileErrorPopup = !loadFragmentShaderFromFile(_filePath);
        }
        ImGui::DragFloat("Scale", &_scale, .01f, 0.0F, std::numeric_limits<float>::max(), "%f");
        ImGui::DragFloat4("Position", (float*)&_offset, .005f, 
            std::numeric_limits<float>::lowest(),
            std::numeric_limits<float>::max(), "%f");
        ImGui::SliderInt("Mouse sensitivity", &_mouseSensitivity, 1, 10);
        if(ImGui::TreeNode("Shader Parameters")){
            userSettingsPanel();
            ImGui::TreePop();
        }
        ImGui::End();

        if(_fileErrorPopup){
            ImGui::Begin("Error", &_fileErrorPopup);
            ImGui::TextColored(ImVec4{1, 0, 0, 1}, "Couldn't load %s: %s",
                _filePath.c_str(), strerror(errno));
            ImGui::End();
        }
    }
    bool loadFragmentShaderFromFile(const std::string& path) {
        std::ifstream f(path, std::ios_base::binary);
        if(!f.is_open()) return 0;

        std::string line;
        std::stringstream ss;
        ss << FRAG_SHADER_HEADER;
        _shaderSettings.clear();
        while(!f.eof()){
            std::getline(f, line);
            if(line.find("uniform") != (size_t)-1){
                auto&& p = parseUniform(line);
                if(p.type == UserParamType_t::unsupported)
                    continue;
                _shaderSettings.emplace_back(parseUniform(line));
            }
            ss << line << '\n';
        }
        _shader = GLBase::Shader(vertexShader, ss.str());
        f.close();
    }
    virtual void draw(const GLBase::Renderer& renderer) const override {
        renderer.draw(_vao, _ibo, _shader);
    }
    ~GLExplorer() = default;
};


int main(int argc, char const *argv[])
{
    std::unique_ptr<GLExplorer> app;
    try
    {
        app = std::make_unique<GLExplorer>();
        app->run();
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        return 1;
    }
    
    return 0;
}
