#version 410 core

uniform sampler2D textureSampler;

in vec2 UV;

//Result
out vec4 fragColor;

void main()
{
    fragColor = texture(textureSampler, UV);
}
