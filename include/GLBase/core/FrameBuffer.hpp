#pragma once

#include "Texture.hpp"

namespace GLBase
{
    class FrameBuffer
    {
    private:
        unsigned _id;
        bool _isBound = false;
    public:
        FrameBuffer();
        void bind() const;
        void unbind() const;
        void attachTexture2D(const Texture& tex, unsigned attachmentIndex);
        unsigned status() const;
        static const char *statusString(unsigned status);
        ~FrameBuffer();
    };
    
} // namespace GLBase
