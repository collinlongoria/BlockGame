#version 460

out vec4 fragColor;

in vec3 normal;
in vec3 fragPos;

uniform vec3 uColor;
uniform vec3 viewPos; // Camera/viewer position

vec3 lightColor = vec3(1.0f, 1.0f, 1.0f);

vec3 lightPos = vec3(-100.0f, 50.0f, 100.0f);

void main()
{
    // Ambient
    float ambientStrength = 0.5;
    vec3 ambient = ambientStrength * lightColor;

    // Diffuse
    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(lightPos - fragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    // Specular
    float specularStrength = 0.5; // Control specular intensity
    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32); // Shininess factor
    vec3 specular = specularStrength * spec * lightColor;

    // Combine results
    vec3 result = (ambient + diffuse + specular) * uColor;
    fragColor = vec4(result, 1.0);
}