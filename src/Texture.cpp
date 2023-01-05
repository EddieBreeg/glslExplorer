#include <GLBase/core/Texture.hpp>
#include <GL/glew.h>
#include <GLBase/core/glErrors.h>

namespace GLBase
{

    Texture::Texture(unsigned width, unsigned height): _w(width), _h(height)
    {
        glCheckCall(glGenTextures(1, &_id));
        bind();
        glCheckCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _w, _h, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr));
        glCheckCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
        glCheckCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
        glCheckCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER));
        glCheckCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER));
        unbind();
        
    }
    void Texture::bind() const {
        glCheckCall(glBindTexture(GL_TEXTURE_2D, _id));
    }
    void Texture::unbind() const {
        glCheckCall(glBindTexture(GL_TEXTURE_2D, 0));
    }
    Texture::~Texture()
    {
    }

} // namespace GLBase
