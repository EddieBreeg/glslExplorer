#pragma once

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

static constexpr const char *defaultFragShader= R"(
#version 450 core
out vec4 color;
in vec4 pos;
void main() { 
    float fac = 1-step(.5, length(pos.xy));
    color = vec4(fac);
}
)";

static constexpr const char *errorFragShader = R"(
#version 450 core
out vec4 color;
void main() { color = vec4(1, 0, 1, 1); }
)";