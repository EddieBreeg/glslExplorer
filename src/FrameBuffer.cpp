#include <GLBase/core/FrameBuffer.hpp>
#include <GL/glew.h>
#include <GLBase/core/glErrors.h>

namespace GLBase
{
    FrameBuffer::FrameBuffer()
    {
        glCheckCall(glGenFramebuffers(1, &_id));
    }
    void FrameBuffer::bind() const 
    {
        glCheckCall(glBindFramebuffer(GL_FRAMEBUFFER, _id));
    }
    void FrameBuffer::unbind() const 
    {
        glCheckCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
    }
    unsigned FrameBuffer::status() const {
        bind();
        glCheckCall(
        GLenum status = 
        glCheckFramebufferStatus(GL_FRAMEBUFFER));
        return status;
    }
    void FrameBuffer::attachTexture2D(const Texture& tex, unsigned index){
        tex.bind();
        bind();
        glCheckCall(
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0+index, GL_TEXTURE_2D,tex.getID(), 0);
        );
        unbind();
    }
    #define FB_STATUS_CASE(name)    case name: return #name
    const char * FrameBuffer::statusString(unsigned status){
        switch (status)
        {
            FB_STATUS_CASE(GL_FRAMEBUFFER_UNDEFINED);
            FB_STATUS_CASE(GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT);
            FB_STATUS_CASE(GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT);
            FB_STATUS_CASE(GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER);
            FB_STATUS_CASE(GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER);
            FB_STATUS_CASE(GL_FRAMEBUFFER_UNSUPPORTED);
            FB_STATUS_CASE(GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE);
            FB_STATUS_CASE(GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS);
        default: return "Unknown";
        }        
    }
    FrameBuffer::~FrameBuffer(){
        glCheckCall(glDeleteFramebuffers(1, &_id));
    }
} // namespace GLBase
