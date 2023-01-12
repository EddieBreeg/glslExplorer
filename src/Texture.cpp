#include <GLBase/core/Texture.hpp>
#include <GL/glew.h>
#include <GLBase/core/glErrors.h>

namespace GLBase
{

    Texture::Texture(unsigned width, unsigned height, GLenum format, GLenum wrapMode):
        _wrapMode(wrapMode)
    {
        glCheckCall(glGenTextures(1, &_id));
        bind();
        glCheckCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
        glCheckCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
        glCheckCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapMode));
        glCheckCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapMode));
        writeData(width, height, format, GL_UNSIGNED_BYTE, nullptr);
    }
    Texture::Texture(unsigned width, unsigned height, GLenum format, GLenum type, const void *data, 
        GLenum  wrapMode)
    {
        glCheckCall(glGenTextures(1, &_id));
        bind();
        glCheckCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
        glCheckCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
        glCheckCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapMode));
        glCheckCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapMode));
        writeData(width, height, format, type, data);
    }
    void Texture::writeData(unsigned width, unsigned height, GLenum format, GLenum type, const void *data){
        _w = width;
        _h = height;
        _format = format;
        _type = type;
        bind();
        glCheckCall(
        glTexImage2D(GL_TEXTURE_2D, 0, format, _w, _h, 0, format, _type, data)
        );        

    }
    void Texture::setWrapMode(GLenum mode){
        bind();
        _wrapMode = mode;
        glCheckCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, _wrapMode));
        glCheckCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, _wrapMode));
    }
    void Texture::setFilterMode(GLenum mode){
        bind();
        glCheckCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, mode));
        glCheckCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mode));
    }
    Texture::Texture(): _w(0), _h(0),_format(GL_RGB), _type(GL_UNSIGNED_BYTE)
    {
        glCheckCall(glGenTextures(1, &_id));
        bind();
        glCheckCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
        glCheckCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
        glCheckCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, _wrapMode));
        glCheckCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, _wrapMode));
    }
    void Texture::resize(unsigned w, unsigned h, GLenum format){
        _format = format;
        bind();
        _w = w;
        _h = h;
        glCheckCall(
        glTexImage2D(GL_TEXTURE_2D, 0, _format, w, h, 0, _format, _type, nullptr)
        );
    }
    void Texture::bind() const {
        glCheckCall(glBindTexture(GL_TEXTURE_2D, _id));
    }
    std::pair<unsigned, unsigned> Texture::getSize() const {
        return {_w, _h};
    }
    void Texture::bind(unsigned unit) const {
        glCheckCall(glActiveTexture(GL_TEXTURE0+unit));
        glCheckCall(glBindTexture(GL_TEXTURE_2D, _id));
    }
    void Texture::unbind() const {
        glCheckCall(glBindTexture(GL_TEXTURE_2D, 0));
    }
    Texture::~Texture()
    {
        glCheckCall(glDeleteTextures(1, &_id));
    }

} // namespace GLBase
