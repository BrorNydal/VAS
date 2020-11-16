#version 410 core

in vec3 fragPosition;
in vec3 normal;

out vec4 fragColor;

//Light
struct DirLight {
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform DirLight light;
uniform vec3 viewLocation;
uniform vec3 color;

void main()
{
    // ambient
    vec3 ambient = light.ambient;
    //light.ambient * material.ambient

    // * (diff * material.diffuse)
    // diffuse
    vec3 norm = normalize(normal);
    vec3 lightDirection = normalize(-light.direction);
    float diff = max(dot(normal, lightDirection), 0.0);
    vec3 diffuse = light.diffuse * diff;

    // specular
    vec3 viewDir = normalize(viewLocation - fragPosition);
    vec3 reflectDir = reflect(-lightDirection, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.f); // last = shininess
    vec3 specular = light.specular * spec;

    vec4 result = vec4((ambient + diffuse + specular), 1.0) * vec4(color, 1.0);

    fragColor = result;
}
