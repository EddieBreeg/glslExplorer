#include <GLBase/core/GLBuffer.hpp>
#include <GL/glew.h>
#include <GLBase/core/glErrors.h>


namespace GLBase
{

    GLBuffer::GLBuffer(const void *data, unsigned size, unsigned target, unsigned usage):
        _target(target)
    {
        glCheckCall(glGenBuffers(1, &_bufferId));
        bind();
        setData(data, size, usage);
    }
    void GLBuffer::setData(const void *data, unsigned size, unsigned usage)
    {
        glCheckCall(glBufferData(_target, size, data, usage));
    }
    void GLBuffer::bind() const{
        glCheckCall(glBindBuffer(_target, _bufferId));
    }
    void GLBuffer::unbind() const{
        glCheckCall(glBindBuffer(_target, 0));
    }

    GLBuffer::~GLBuffer(){
        glDeleteBuffers(1, &_bufferId);
    }
    
} // namespace GLBase
