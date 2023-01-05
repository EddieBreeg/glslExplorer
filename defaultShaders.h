#pragma once

#define FRAG_SHADER_HEADER  "#version 450 core\n"\
"layout(location=0) out vec4 fragColor; \n"\
"in vec4 pos;\n"\
"uniform float scale;\n"\
"uniform vec4 offset;\n"\
"uniform float time;\n"\
"uniform vec2 res;\n"\
"\n#line 0\n\n"

static constexpr const char *vertexShader = R"(
#version 450 core
layout(location=0) in vec4 aPos;

out vec4 pos;

void main()
{
    gl_Position = aPos;
    pos = aPos;
}
)";

static constexpr const char *defaultFragShader= FRAG_SHADER_HEADER 
    "void main(){ fragColor = vec4(0); }";

static constexpr const char *errorFragShader = 
    FRAG_SHADER_HEADER "void main(){fragColor = vec4(1, 0, 1, 1);}";