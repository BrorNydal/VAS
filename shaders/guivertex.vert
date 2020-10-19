#version 410 core

layout(location = 0) in vec3 position;
layout(location = 2) in vec2 texCoord;

out vec2 UV;

uniform mat4 matrix;
//matrix *
void main(){
    UV = texCoord;
    gl_Position = matrix * vec4(position.y, position.z, -1.0, 1.0);    
}
