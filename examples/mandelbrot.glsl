void main(){
    vec2 z_n = vec2(0);
    for(int i = 0; i < 256; ++i){
        z_n = vec2(z_n.x*z_n.x - z_n.y * z_n.y, 2*z_n.x*z_n.y) + pos.xy;
        if((z_n.x * z_n.x + z_n.y * z_n.y) >= 4) {
            color = vec4(i / 256);
            return;
        }
    }
    color = vec4(1);
}