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
#include <vector>

class GLExplorer: public GLBase::Application
{
private:
    GLBase::VertexBuffer _vbo;
    GLBase::IndexBuffer _ibo;
    GLBase::VertexArray _vao;
    GLBase::Shader _shader;
    std::string _filePath;
    std::vector<UserParam> _shaderSettings;
    GLBase::FrameBuffer _fbo;
    GLBase::Texture _tex;

    bool _vsync = 0;
    std::chrono::steady_clock::time_point _start;
    float _scale = 1.0f;
    int _mouseSensitivity = 4;
    float _sliderSpeed = 1.0f;
    bool _fileErrorPopup = false;
    struct { float x, y, z, w; } _offset = {};
    struct float2 { float x, y; } _mouseMotion;
    struct {std::string message; bool errStatus = false; } _glslStatus;
public:
    GLExplorer(): GLBase::Application(500, 500, "GLSL Explorer", true, true),
        _vbo(nullptr, 3*sizeof(GLBase::Vector3), GL_STATIC_DRAW),
        _ibo(nullptr, 6, GL_STATIC_DRAW),
        _vao(), _shader(vertexShader, defaultFragShader), _tex(500, 500)
    {
        GLBase::Shader::setGLSLErrorCallback(onGLSLError, this);
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
        _fbo.attachTexture2D(_tex, 0);
        unsigned status = _fbo.status();
        if(status != GL_FRAMEBUFFER_COMPLETE) {
            std::cout << GLBase::FrameBuffer::statusString(status) << '\n';
            _fbo.unbind();
        }
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
        _shader.bind();
        std::chrono::duration<float> time = std::chrono::steady_clock::now() - _start;
        _shader.setUniform("res", float(size.first), float(size.second));
        _shader.setUniform("time", time.count());
        _shader.setUniform("scale", _scale);
        _shader.setUniform("offset", _offset.x, _offset.y, _offset.z, _offset.w);
        updateUI();
    }
    float2 getMousePos(){
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
            ImGui::DragFloat(val.name.c_str(), val.value.f, _sliderSpeed, 
                std::numeric_limits<float>::lowest(),
                std::numeric_limits<float>::max());
            _shader.setUniform(val.name, val.value.f[0]);
            break;
        case UserParamType_t::f2:
            ImGui::DragFloat2(val.name.c_str(), val.value.f, _sliderSpeed, 
                std::numeric_limits<float>::lowest(),
                std::numeric_limits<float>::max());
            _shader.setUniform(val.name, val.value.f[0], val.value.f[1]);
            break;
        case UserParamType_t::f3:
            ImGui::DragFloat3(val.name.c_str(), val.value.f, _sliderSpeed, 
                std::numeric_limits<float>::lowest(),
                std::numeric_limits<float>::max());
            _shader.setUniform(val.name, val.value.f[0], val.value.f[1], val.value.f[2]);
            break;
        case UserParamType_t::f4:
            ImGui::DragFloat4(val.name.c_str(), val.value.f, _sliderSpeed, 
                std::numeric_limits<float>::lowest(),
                std::numeric_limits<float>::max());
            _shader.setUniform(val.name, val.value.f[0], val.value.f[1], val.value.f[2], val.value.f[3]);
            break;
        case UserParamType_t::i1:
            ImGui::InputInt(val.name.c_str(), val.value.i, 1, 10);
            _shader.setUniform(val.name, val.value.i[0]);
            break;
        case UserParamType_t::i2:
            ImGui::InputInt2(val.name.c_str(), val.value.i);
            _shader.setUniform(val.name, val.value.i[0], val.value.i[1]);
            break;
        case UserParamType_t::i3:
            ImGui::InputInt3(val.name.c_str(), val.value.i);
            _shader.setUniform(val.name, val.value.i[0], val.value.i[1], val.value.i[2]);
            break;
        case UserParamType_t::i4:
            ImGui::InputInt4(val.name.c_str(), val.value.i);
            _shader.setUniform(val.name, val.value.i[0], val.value.i[1], val.value.i[2], val.value.i[3]);
            break;
        case UserParamType_t::b1:
            ImGui::Checkbox(val.name.c_str(), &val.value.b);
            _shader.setUniform(val.name, val.value.b);
            break;
        case UserParamType_t::b2:
            ImGui::DragInt2(val.name.c_str(), val.value.i, _sliderSpeed, 0, 1);
            _shader.setUniform(val.name, val.value.i[0], val.value.i[1]);
            break;
        case UserParamType_t::b3:
            ImGui::DragInt3(val.name.c_str(), val.value.i, _sliderSpeed, 0, 1);
            _shader.setUniform(val.name, val.value.i[0], val.value.i[1], val.value.i[2]);
            break;
        case UserParamType_t::b4:
            ImGui::DragInt4(val.name.c_str(), val.value.i, _sliderSpeed, 0, 1);
            _shader.setUniform(val.name, val.value.i[0], val.value.i[1], val.value.i[2], val.value.i[3]);
            break;
        default:
            break;
        }
    }
    void userSettingsPanel(){
        for(auto& p: _shaderSettings)
            dispUserSettingValue(p);
    }
    static void onGLSLError(const char *msg, void *data){
        GLExplorer *app = (GLExplorer*)data;
        app->_glslStatus.errStatus = false;
        app->_glslStatus.message = msg;
    }
    void updateUI() {
        ImGui::Begin("Inspector");
        ImGui::Text("%f FPS", ImGui::GetIO().Framerate);
        if(ImGui::TreeNode("General settings")){
            stringInput("Shader file", _filePath);
            if(ImGui::Button("Load")){
                _fileErrorPopup = !loadFragmentShaderFromFile(_filePath);
            }
            ImGui::SameLine();
            if(ImGui::Button("Reset time")){
                _start = std::chrono::steady_clock::now();
            }
            ImGui::DragFloat("Scale", &_scale, .01f, 0.0F, std::numeric_limits<float>::max(), "%f");
            ImGui::DragFloat4("Position", (float*)&_offset, .005f, 
                std::numeric_limits<float>::lowest(),
                std::numeric_limits<float>::max(), "%f");
            ImGui::SliderInt("Mouse sensitivity", &_mouseSensitivity, 1, 20);
            ImGui::SliderFloat("Slider sensitivity", &_sliderSpeed, 0.0f, 1);
            if(ImGui::Checkbox("Vsync", &_vsync))
                glfwSwapInterval(_vsync);
            ImGui::TreePop();
        }
        if(ImGui::TreeNode("Shader Parameters")){
            userSettingsPanel();
            ImGui::TreePop();
        }
        ImGui::End();
        if(_glslStatus.errStatus){
            ImGui::Begin("GLSL Error");
            ImGui::TextColored(ImVec4{1, 0, 0, 1}, "%s", _glslStatus.message.c_str());
            ImGui::End();
        }
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
                _shaderSettings.emplace_back(std::move(p));
            }
            ss << line << '\n';
        }
        _shader = GLBase::Shader(vertexShader, ss.str());
        _glslStatus.errStatus = !_shader.isValid();
        f.close();
        return 1;
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
