#include <GLBase/GLBase.hpp>
#include <memory>
#include <iostream>
#include "defaultShaders.h"
#include <imgui/imgui.h>
#include <numeric>

class GLExplorer: public GLBase::Application
{
private:
    GLBase::VertexBuffer _vbo;
    GLBase::IndexBuffer _ibo;
    GLBase::VertexArray _vao;
    GLBase::Shader _shader;

    float _scale = 1.0f;
    struct { float x, y, z, w; } _offset = {};
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
    virtual void update() override {
        if(!_shader.isValid()){
            _shader.setSource(vertexShader, errorFragShader);
        }
        _shader.bind();
        auto size = windowSize();
        float ratio = float(size.first) / float(size.second);
        _shader.setUniform("uRatio", ratio);
        updateUI();
    }
    void updateUI() {
        ImGui::Begin("Settings");
        if(ImGui::DragFloat("Scale", &_scale, .1, 0.0F, std::numeric_limits<float>::max(), "%f", 1.0f))
        {
            _shader.setUniform("uScale", _scale);
        }
        if(ImGui::DragFloat4("Position", (float*)&_offset, .1, 
            std::numeric_limits<float>::lowest(),
            std::numeric_limits<float>::max(), "%f", 1.0f))
            _shader.setUniform("uOffset", _offset.x, _offset.y, _offset.z, _offset.w);
        ImGui::End();
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
