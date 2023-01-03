uniform vec4 uColor = vec4(1);

void main(){
    vec2 z_n = vec2(0);
    int i = 0;
    while(i < 256 && dot(z_n, z_n) < 4){
        z_n = vec2(z_n.x * z_n.x - z_n.y * z_n.y, 2 * z_n.x * z_n.y) + vec2(pos.xy);
        ++i;
    }
    float fac = float(i) / 256.0;
    color = fac * uColor;
}