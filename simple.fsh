void main(void)
{
    vec4 color = vec4(0.0, 0.0, 0.0, 1.0);
    color.y = mod(gl_FragCoord.y*0.01, 1.0);
    gl_FragColor = color;
}
