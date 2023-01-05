#pragma once

#include <inttypes.h>

#ifndef  __glew_h__
#define GL_CLAMP_TO_BORDER 0x812D
#endif

namespace GLBase
{
    class Texture
    {
    private:
        unsigned _id;
        unsigned _w, _h;
        unsigned _format;
        unsigned _type;
        unsigned _wrapMode = GL_CLAMP_TO_BORDER;
    public:
        struct RGBAPixel
        {
            uint8_t r, g, b, a;
        };
        
        Texture(unsigned width, unsigned height, unsigned format, unsigned wrapMode = GL_CLAMP_TO_BORDER);
        Texture(unsigned width, unsigned height, unsigned format, unsigned type, const void* data,
            unsigned wrapMode = GL_CLAMP_TO_BORDER);
        Texture();
        void setWrapMode(unsigned mode);
        void setFilterMode(unsigned mode);
        void resize(unsigned w, unsigned h);
        void writeData(unsigned width, unsigned height, unsigned format, unsigned type, const void* data);
        void bind() const;
        void bind(unsigned unit) const;
        void unbind() const;
        unsigned getID() const { return _id; }
        ~Texture();
    };
} // namespace GLBase

#ifndef __glew_h__
#undef GL_CLAMP_TO_BORDER
#endif

