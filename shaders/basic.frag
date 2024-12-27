#version 460

out vec4 fragColor;

in vec3 normal;
in vec3 fragPos;

uniform vec3 uColor;

vec3 lightColor = vec3(1.0f, 1.0f, 1.0f);

vec3 lightPos = vec3(-100.0f, 50.0f, 100.0f);

void main()
{
    // ambient
    float ambientStrength = 0.5;
    vec3 ambient = ambientStrength * lightColor;

    // Diffuse
    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(lightPos - fragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    vec3 result = (ambient + diffuse) * uColor;
    fragColor = vec4(result, 1.0);
}