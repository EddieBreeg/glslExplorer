#include <GLBase/GLBase.hpp>
#include <memory>
#include <iostream>
#include "defaultShaders.h"
#include <imgui/imgui.h>
#include <numeric>
#include <errno.h>
#include <stdio.h>
#include <sstream>
#include <filesystem>
#include <cmath>

class GLExplorer: public GLBase::Application
{
private:
    GLBase::VertexBuffer _vbo;
    GLBase::IndexBuffer _ibo;
    GLBase::VertexArray _vao;
    GLBase::Shader _shader;
    std::string _filePath;

    float _scale = 1.0f;
    bool _fileErrorPopup = false;
    struct { float x, y, z, w; } _offset = {};
    struct { float x, y; } _mouseMotion;
public:
    GLExplorer(): GLBase::Application(500, 500, "GLSL Explorer", true, true),
        _vbo(nullptr, 3*sizeof(GLBase::Vector3), GL_STATIC_DRAW),
        _ibo(nullptr, 6, GL_STATIC_DRAW),
        _vao(), _shader(vertexShader, defaultFragShader)
    {
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
        static auto mouseOld = getMousePos();
        auto mouseNew = getMousePos();
        _mouseMotion = {mouseNew.x-mouseOld.x, mouseOld.y-mouseNew.y}; // y axis is inverted
        mouseOld = mouseNew;
        if(glfwGetMouseButton(_window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS){
            float speed = 1000.f * _scale * (float)delta.count();
            if(glfwGetKey(_window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS){                
                _scale *= expf(-_mouseMotion.x * (float)delta.count());
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
        _shader.setUniform("uRatio", ratio);
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
    static void filePathInput(const char *label, std::string& out){
        ImGui::InputText(label, out.data(), out.capacity(), 
            ImGuiInputTextFlags_CallbackResize, textSizeCallback, (void*)&out);
    }
    void updateUI() {
        ImGui::Begin("Settings");
        filePathInput("Shader file", _filePath);
        if(ImGui::Button("Load")){
            _fileErrorPopup = !loadFragmentShaderFromFile(_filePath);
        }
        ImGui::DragFloat("Scale", &_scale, .01f, 0.0F, std::numeric_limits<float>::max(), "%f");
        ImGui::DragFloat4("Position", (float*)&_offset, .005f, 
            std::numeric_limits<float>::lowest(),
            std::numeric_limits<float>::max(), "%f");
        ImGui::End();

        if(_fileErrorPopup){
            ImGui::Begin("Error", &_fileErrorPopup);
            ImGui::TextColored(ImVec4{1, 0, 0, 1}, "Couldn't load %s: %s",
                _filePath.c_str(), strerror(errno));
            ImGui::End();
        }
    }
    bool loadFragmentShaderFromFile(const std::string& path) {
        FILE *f = fopen(path.c_str(), "rb");
        if(!f) return 0;
        
        auto size = std::filesystem::file_size(path);
        const size_t headerSize = std::end(FRAG_SHADER_HEADER) - FRAG_SHADER_HEADER;
        std::vector<char> source(headerSize+size);
        char *buf = std::copy(FRAG_SHADER_HEADER, FRAG_SHADER_HEADER+headerSize-1, source.data());
        source.back() = 0;
        size_t nRead=0;
        do
        {
            size_t k = fread(buf, 1, size-nRead, f);
            buf += k;
            if (!k) return 0;
            nRead += k;
        } while(nRead != size);
        _shader = GLBase::Shader(vertexShader, {source.data(), size+1});
        fclose(f);
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
