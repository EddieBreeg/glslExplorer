#include <GLBase/core/IndexBuffer.hpp>
#include <GL/glew.h>

namespace GLBase
{
    IndexBuffer::IndexBuffer(const uint32_t *data, unsigned count, unsigned usage):
        GLBuffer(data, sizeof(uint32_t) * count, GL_ELEMENT_ARRAY_BUFFER, usage), _count(count)
    {}
    void IndexBuffer::setIndexData(const uint32_t *data, unsigned count, unsigned usage){
        setData(data, count*sizeof(uint32_t), usage);
    }
    IndexBuffer::~IndexBuffer() {}

    unsigned IndexBuffer::count() const { return _count; }
} // namespace GLBase
