uniform float r = .5;
uniform float smoothness = .007;
uniform vec4 color = vec4(1, .5, 0, 1);

void main() { 
    vec2 p = vec2(pos.x*res.x/res.y, pos.y) * scale + offset.xy;
    float fac = 1-smoothstep(r, (1+smoothness)*r, length(p));
    fragColor = fac * color;
}