#include <GLBase/core/VertexBuffer.hpp>
#include <GL/glew.h>

namespace GLBase
{
    VertexBuffer::VertexBuffer(const void *data, unsigned size, unsigned usage):
        GLBuffer(data, size, GL_ARRAY_BUFFER, usage)
    {
    }
    void VertexBuffer::setVertexData(const void *data, unsigned size, unsigned usage)
    {
        setData(data, size, usage);
    }
    VertexBuffer::~VertexBuffer(){
    }
} // namespace GLBase
