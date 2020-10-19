#version 410 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 uv;


out vec3 textureCoords;

uniform mat4 pmatrix;
uniform mat4 vmatrix;

void main(){
    gl_Position = pmatrix * vmatrix * vec4(position, 1.0);
    textureCoords = uv;
}
