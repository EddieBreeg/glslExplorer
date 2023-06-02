uniform float speed = 1.0;
uniform float randomness = 1.0;
uniform vec3 lightSource = vec3(0, 0, -1);
uniform float lightAttenuation = 1.0;
uniform float lightPower = 1.0;
uniform vec4 baseColor = vec4(1, 1, 1, 1);

float chebichev(vec3 x){
    x = abs(x);
    return max(x.x, max(x.y, x.z));
}

const vec3 offsets[] = {
    vec3(-1, -1, -1),
    vec3(-1, -1, 0),
    vec3(-1, -1, 1),
    vec3(-1, 0, -1),
    vec3(-1, 0, 0),
    vec3(-1, 0, 1),
    vec3(-1, 1, -1),
    vec3(-1, 1, 0),
    vec3(-1, 1, 1),
    vec3(0, -1, -1),
    vec3(0, -1, 0),
    vec3(0, -1, 1),
    vec3(0, 0, -1),
    vec3(0, 0, 0),
    vec3(0, 0, 1),
    vec3(0, 1, -1),
    vec3(0, 1, 0),
    vec3(0, 1, 1),
    vec3(1, -1, -1),
    vec3(1, -1, 0),
    vec3(1, -1, 1),
    vec3(1, 0, -1),
    vec3(1, 0, 0),
    vec3(1, 0, 1),
    vec3(1, 1, -1),
    vec3(1, 1, 0),
    vec3(1, 1, 1),
};

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

float voronoi(vec3 P, out vec4 outColor){
    vec3 S = floor(P);
    vec3 F = fract(P);
    float d = 2;
    for(int i=0; i<27; ++i){
        vec3 t = offsets[i];
        vec3 cell = S+t;
        vec3 p = randomness*hash(cell);
        float r = chebichev(p+t - F);
        if(r < d){
            d = r;
        }
    }
    return d;
}

void main()
{
    vec3 P = vec3(pos.xy * scale, 0) + offset.xyz;
    P.x *= res.x/res.y;
    vec4 randColor;

    vec4 temp;
    float d = voronoi(P, randColor);
    vec3 dx = vec3(2/(res.x), 0, 0);
    vec3 dy = vec3(0, 2/(res.y), 0);
    float R = voronoi(P+dx, temp);
    float B = voronoi(P-dy, temp);
    vec3 normal = normalize(vec3(
        (d-R)/dx.x,
        (B-d)/dy.y,
        -1));
    frag1 = vec4(.5 * normal + .5, 1);
    vec3 lightOffset = vec3(cos(speed*time), sin(speed*time), 0);
    float lightDistance = length(lightSource+lightOffset - P);
    float lightValue = dot(normalize(lightSource+lightOffset - P), normal) * lightPower 
        / pow(lightDistance, lightAttenuation);
    frag2 =  baseColor * lightValue;

    
    frag0 = vec4(d);
}