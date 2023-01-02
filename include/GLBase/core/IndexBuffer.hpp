#pragma once
#include <inttypes.h>
#include "GLBuffer.hpp"

namespace GLBase
{
    class IndexBuffer: public GLBuffer
    {
    private:
        unsigned _count;
    public:
        IndexBuffer(const uint32_t *data, unsigned count, unsigned usage);
        void setIndexData(const uint32_t *data, unsigned count, unsigned usage);
        unsigned count() const;
        ~IndexBuffer();
    };
    
} // namespace GLBase
