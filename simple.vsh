attribute highp vec4 posAttr;
uniform highp mat4 matrix;

void main() {
// posAttr.z = sin(posAttr.x) * 0.1;
//    posAttr.z = 0.0;
    gl_Position = matrix * posAttr;
}
