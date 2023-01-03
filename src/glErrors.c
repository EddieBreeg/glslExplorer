#include <GLBase/core/glErrors.h>
#include <GL/glew.h>

#include <stdio.h>

#define TEXT_STYLE_ERROR        "\033[31;1m"
#define TEXT_STYLE_DEFAULT      "\033[0m"

#define GL_ERROR_ENUM_CASE(val) case val: return #val

const char *glGetErrorString(GLenum err){
    switch (err)
    {
        GL_ERROR_ENUM_CASE(GL_INVALID_ENUM);
        GL_ERROR_ENUM_CASE(GL_INVALID_FRAMEBUFFER_OPERATION);
        GL_ERROR_ENUM_CASE(GL_INVALID_INDEX);
        GL_ERROR_ENUM_CASE(GL_INVALID_VALUE);
        GL_ERROR_ENUM_CASE(GL_INVALID_OPERATION);
    default: return "Unknown";
    }
}

int glLogError(const char *funcName, const char *file, int lineNumber){
    unsigned err = glGetError();
    if(!err) return 0;
    const char *msg = glGetErrorString(err);
    printf(TEXT_STYLE_ERROR "Error [%s] at %s:%i: %s\n" TEXT_STYLE_DEFAULT, msg, file, lineNumber, funcName);
    return 1;
}