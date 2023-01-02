#include <GLBase/core/VertexArray.hpp>
#include <GLBase/core/glErrors.h>
#include <GL/glew.h>

namespace GLBase
{
    VertexArray::VertexArray()
    {
        glCheckCall(glGenVertexArrays(1, &_arrayId));
    }

    VertexArray::~VertexArray()
    {
        glDeleteVertexArrays(1, &_arrayId);
    }
    void VertexArray::bind() const {
        glCheckCall(glBindVertexArray(_arrayId));
    }
    void VertexArray::unbind() const {
        glCheckCall(glBindVertexArray(0));
    }    
} // namespace GLBase


