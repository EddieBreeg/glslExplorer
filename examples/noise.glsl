float hash(vec4 x) {
    vec4 p = vec4(19.474959781, 51.422209728, 57.450435988, 777.79154659);
    return fract(9581.400139252703 * sin(dot(x, p)));
 }
vec4 hash4(vec4 x){
    return vec4(
        hash(x),
        hash(vec4(x.y, x.z, x.w, x.x)),
        hash(vec4(x.z, x.w, x.x, x.y)),
        hash(vec4(x.w, x.x, x.y, x.z))
    );
}

void main()
{
    vec4 p = (scale*pos+offset)/scale;
    fragColor = vec4(hash4(p).xyz, 1);
}