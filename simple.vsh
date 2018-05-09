attribute highp vec4 posAttr;
uniform highp mat4 matrix;

void main() {
// posAttr.z = sin(posAttr.x) * 0.1;

    vec4 pos = posAttr.xyzw;
    pos.z += 0.5*sin(20.0*pos.x);
    gl_Position = matrix * pos;
}
