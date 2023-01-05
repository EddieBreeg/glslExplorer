#pragma once

#define FRAG_SHADER_HEADER  "#version 450 core\n"\
"layout(location=0) out vec4 fragColor; \n"\
"layout(location=1) out vec3 fragNormal; \n"\
"in vec4 pos;\n"\
"in vec2 uv;\n" \
"uniform float scale;\n"\
"uniform vec4 offset;\n"\
"uniform float time;\n"\
"uniform vec2 res;\n"\
"\n#line 0\n\n"

static constexpr const char *vertexShader = R"(
#version 450 core
layout(location=0) in vec4 aPos;

out vec4 pos;
out vec2 uv;

void main()
{
    gl_Position = aPos;
    pos = aPos;
    uv = aPos.xy / 2 + .5;
}
)";

static constexpr const char *defaultFragShader= FRAG_SHADER_HEADER 
    "void main(){ fragColor = vec4(uv.xy+offset.xy, 0, 1); fragNormal=vec3(0, 0, 1); }";

static constexpr const char *errorFragShader = 
    FRAG_SHADER_HEADER "void main(){fragColor = vec4(1, 0, 1, 1);}";

static constexpr const char *simpleTextureShader = 
    "#version 450 core\n"
    "in vec2 uv;\n"
    "in vec4 pos;\n"
    "layout(location=0) out vec4 color;\n"
    "uniform sampler2D tex;\n"
    "void main(){ color = texture(tex, uv.xy);}";