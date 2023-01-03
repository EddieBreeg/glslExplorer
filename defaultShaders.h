#pragma once

#define FRAG_SHADER_HEADER  "#version 450 core\nout vec4 color; \nin vec4 pos;\n"\
"uniform float time;\n"\
"\n#line 1\n\n"

static constexpr const char *vertexShader = R"(
#version 450 core
layout(location=0) in vec4 aPos;
out vec4 pos;
uniform float uRatio = 1.0;
uniform vec4 uOffset;
uniform float uScale = 1.0;

void main()
{
    gl_Position = aPos;
    pos = uScale * vec4(uRatio * aPos.x, aPos.yzw) + uOffset;
}
)";

static constexpr const char *defaultFragShader= FRAG_SHADER_HEADER 
    "void main(){ color = vec4(0); }";

static constexpr const char *errorFragShader = 
    FRAG_SHADER_HEADER "void main(){color = vec4(1, 0, 1, 1);}";