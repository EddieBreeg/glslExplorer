#include <GLBase/core/Renderer.hpp>
#include <GL/glew.h>
#include <GLBase/core/glErrors.h>


namespace GLBase
{
    void Renderer::draw(const VertexArray& vArray, const IndexBuffer& iBuffer, const Shader& shader) const{
        vArray.bind();
        iBuffer.bind();
        shader.bind();

        glCheckCall(glDrawElements(GL_TRIANGLES, iBuffer.count(), GL_UNSIGNED_INT, nullptr));
    }

    void Renderer::clear() const { glCheckCall(glClear(GL_COLOR_BUFFER_BIT)); }
    
} // namespace GLBase
