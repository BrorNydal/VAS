#version 410 core

vec4 color;

float heightCurve = 1.5;
float heightCurveThickness = 0.015;

in vec3 fragPosition;
in vec3 normal;

uniform vec3 viewPosition;

out vec4 fragColor;

//Light
uniform float lightIntensity;
uniform vec3 lightColor;
uniform vec3 lightPosition;

void main()
{
    color = vec4(0.0, 0.7, 0.7, 1.0);

    // ambient
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * (lightColor);

    // diffuse
    vec3 norm = normalize(normal);
    vec3 lightDirection = normalize(lightPosition - fragPosition);
    float diff = max(dot(normal, lightDirection), 0.0);
    vec3 diffuse = diff * lightColor;

    // specular
    float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPosition - fragPosition);
    vec3 reflectDir = reflect(-lightDirection, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;

    vec3 result = (ambient + diffuse + specular) * lightIntensity;
    fragColor = vec4(result, 1.0) * color;
}
