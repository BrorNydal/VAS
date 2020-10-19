#version 410 core

//Material

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 speccular;
    float shininess;
};

uniform Material material;

//Light
struct Light {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 speccular;

    float constant;
    float linear;
    float quadratic;
};

uniform Light light;

//Color
uniform int useTexture;
vec4 color = vec4(0,0,0,1);
uniform sampler2D textureSampler;

//Vertex shader
in vec3 fragPosition;
in vec3 normal;
in vec2 UV;

//Camera
uniform vec3 viewPosition;

//Result
out vec4 fragColor;

void main()
{
    float distance = length(light.position - fragPosition);
    float attenuation = 1.0 / (light.constant + light.linear * distance +
                        light.quadratic * (distance * distance));

    // ambient
    vec3 ambient = light.ambient * material.ambient;
    //light.ambient * material.ambient

    // * (diff * material.diffuse)
    // diffuse
    vec3 norm = normalize(normal);
    vec3 lightDirection = normalize(light.position - fragPosition);
    float diff = max(dot(normal, lightDirection), 0.0);
    vec3 diffuse = light.diffuse * (diff * material.diffuse);//texture(textureSampler, UV).rgb;

    // specular
    float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPosition - fragPosition);
    vec3 reflectDir = reflect(-lightDirection, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.speccular * (spec * material.speccular);

    if(useTexture == 1)
    {
        ambient *= texture(textureSampler, UV).rgb;
        diffuse *= texture(textureSampler, UV).rgb;
        specular *= texture(textureSampler, UV).rgb;
    }

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    vec3 result = (ambient + diffuse + specular);
    fragColor = vec4(result, 1.0);
}
