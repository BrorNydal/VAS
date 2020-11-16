#version 410 core

layout(location = 0) in vec3 posAttr;
layout(location = 1) in vec3 colAttr;

out vec4 col;

uniform vec3 color = vec3(0.0,0.0,0.0);
uniform mat4 matrix;
uniform mat4 pmatrix;
uniform mat4 vmatrix;

void main() {
    if(color.x > 0.0 || color.y > 0.0 || color.z > 0.0)
        col = vec4(color, 1.0);
    else
        col = vec4(colAttr, 1.0);

    gl_Position = pmatrix * vmatrix * matrix * vec4(posAttr, 1.0);
}
