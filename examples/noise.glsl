uvec3 pcg3d(uvec3 v)
{
    v = v * 1664525u + 1013904223u;
    v.x += v.y*v.z; v.y += v.z*v.x; v.z += v.x*v.y;
    v ^= v >> 16u;
    v.x += v.y*v.z; v.y += v.z*v.x; v.z += v.x*v.y;
    return v;
}

vec3 hash(vec3 p){
    return pcg3d(floatBitsToUint(p)) / 4294967295.0;
}

void main()
{
    vec4 p = scale*pos+offset;
    p.x *= res.x / res.y;
    p = floor(p);
    frag0 = vec4(hash(p.xyz), 1);
}