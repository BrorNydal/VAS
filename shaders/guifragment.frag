#version 410 core

in vec2 UV;
uniform sampler2D guiTexture;

//Result
out vec4 fragColor;

void main(){
    fragColor = texture(guiTexture, UV);
}
