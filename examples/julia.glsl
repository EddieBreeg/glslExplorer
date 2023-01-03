uniform vec2 C0;
uniform float R;
uniform float speed;

vec2 f(vec2 z, vec2 C){
    return vec2(z.x*z.x - z.y*z.y, 2*z.x*z.y) + C;
}

void main(){
    int i = 0;
    vec2 z = pos.xy;
    vec2 C = C0*C0*C0;
    while(i++ < 256){
        z = f(z, C + R*vec2(cos(speed*time), sin(speed*time)));
        if(dot(z, z) > (4)){
            color = vec4(0);
            return;
        }
    }
    color = float(i)/256. * vec4(1, 0, 0, 1);
}