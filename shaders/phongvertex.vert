#version 410 core

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec3 vertexNormal;
layout(location = 2) in vec2 textureCoordinates;

out vec3 fragPosition;
out vec3 normal;
out vec2 UV;

uniform mat4 matrix;
uniform mat4 vmatrix;
uniform mat4 pmatrix;

void main() {
    UV = textureCoordinates;

    gl_Position = pmatrix * vmatrix * matrix * vec4(vertexPosition, 1.0);

    fragPosition = vec3(matrix * vec4(vertexPosition, 1.0));
    normal = mat3(transpose(inverse(matrix))) * vertexNormal;
}

