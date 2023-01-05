uniform float speed = 1.0;
uniform float randomness = 1.0;
uniform bvec2 foobar = bvec2(true, false);

vec3 hash(vec3 x){
    mat3x3 M = mat3x3(77.00952309, 94.48812373, 49.46552442,
    75.89344076, 99.45643466, 14.34464313,
    54.57706595, 18.21759071, 68.77093474);
    return fract(489.02821708762366 * sin(M * x));
}

float chebichev(vec3 x){
    x = abs(x);
    return max(x.x, max(x.y, x.z));
}

void main()
{
    vec3 P = vec3(pos.xy * scale, time*speed) + offset.xyz;
    P.x *= res.x/res.y;
    vec3 S = floor(P);
    vec3 F = fract(P);
    float d = 2;
    vec4 randColor;
    for(int x=-1; x<2; ++x){
        for(int y=-1; y<2; ++y){
            for(int z=-1; z < 2; ++z){
                vec3 t = vec3(x, y, z);
                vec3 p = randomness*hash(S+t);
                float r = chebichev(p+t - F);
                if(r < d){
                    d = r;
                    randColor = vec4(hash(S+t).xyz, 1);
                }
            }
        }
    }
    fragColor = randColor;
}