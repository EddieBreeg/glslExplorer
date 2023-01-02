#include <GLBase/core/glErrors.h>
#include <GL/glew.h>

#include <stdio.h>

#define TEXT_STYLE_ERROR        "\033[31;1m"
#define TEXT_STYLE_DEFAULT      "\033[0m"

int glLogError(const char *funcName, const char *file, int lineNumber){
    unsigned err = glGetError();
    if(!err) return 0;
    fprintf(stderr, TEXT_STYLE_ERROR
    "[OpenGL Error (code 0x%x)] " 
    "At %s:%d: %s" TEXT_STYLE_DEFAULT "\n", err, file, lineNumber, funcName);
    return 1;
}