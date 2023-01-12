uniform int maxIter = 256;
uniform float R = 4;

vec4 palette[16] = {
    vec4(0.24595138, 0.66295644, 0.63899203, 1),
    vec4(0.44023494, 0.6728249 , 0.98855336, 1),
    vec4(0.83858466, 0.99083222, 0.16504827, 1),
    vec4(0.97067473, 0.94837752, 0.31209835, 1),
    vec4(0.19798793, 0.5018788 , 0.60514655, 1),
    vec4(0.27590692, 0.27210327, 0.21077567, 1),
    vec4(0.21717932, 0.11664078, 0.14657013, 1),
    vec4(0.19436156, 0.48113588, 0.91326362, 1),
    vec4(0.39223649, 0.8480419 , 0.84402102, 1),
    vec4(0.35429426, 0.85522943, 0.37495551, 1),
    vec4(0.55353182, 0.55738413, 0.14343645, 1),
    vec4(0.84117312, 0.48480415, 0.13428883, 1),
    vec4(0.61420576, 0.95770609, 0.85899091, 1),
    vec4(0.09168334, 0.57447571, 0.53334733, 1),
    vec4(0.37780096, 0.84848275, 0.76712196, 1),
    vec4(0.54188937, 0.60923355, 0.54239134, 1),
};

void main(){
    vec2 C = pos.xy * scale;
    C.x *= res.x/res.y;
    C += offset.xy;
    vec2 Z = vec2(0);
    int i = 0;
    while(i++ < maxIter && dot(Z, Z) <= 4){
        Z = vec2(Z.x * Z.x - Z.y * Z.y, 2*Z.x*Z.y) + C;
    }
    int index = int(round(float(i) / float(maxIter) * 16));
    frag0 = palette[index];
}