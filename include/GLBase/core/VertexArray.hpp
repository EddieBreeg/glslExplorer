#pragma once

#include <GL/glew.h>
#include "VertexBuffer.hpp"
#include "VertexBufferLayout.hpp"
#include "glErrors.h"
#include <cstdint>


namespace GLBase
{
    class VertexArray
    {
    private:
        unsigned _arrayId;
    public:
        VertexArray();
        template<unsigned N>
        void addBuffer(const VertexBuffer& buff, const VertexLayout<N>& layout) const {
            bind();
            buff.bind();
            uintptr_t offset = 0;
            unsigned stride = layout.stride();
            for (unsigned i = 0; i < N; i++)
            {
                const auto& e = layout[i];
                glCheckCall(glVertexAttribPointer(i, e.count(), e.type(), e.normalized(), stride, (void*)offset))
                glCheckCall(glEnableVertexAttribArray(i));
                offset += e.size();
            }
        }
        void bind() const;
        void unbind() const;
        ~VertexArray();
    };    
} // namespace GLBase


