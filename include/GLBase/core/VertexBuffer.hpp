#pragma once
#include "GLBuffer.hpp"


namespace GLBase
{
    class VertexBuffer: public GLBuffer
    {
    public:
        VertexBuffer(const void *data, unsigned size, unsigned usage);
        void setVertexData(const void *data, unsigned size, unsigned usage);
        ~VertexBuffer();
    };
} // namespace GLBase
