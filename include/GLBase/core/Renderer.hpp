#pragma once

#include "Shader.hpp"
#include "VertexArray.hpp"
#include "IndexBuffer.hpp"

namespace GLBase
{
    class Renderer
    {
    public:
        void draw(const VertexArray& vArray, const IndexBuffer& iBuffer, const Shader& shader) const;
        void clear() const;
    };
    
} // namespace GLBase
