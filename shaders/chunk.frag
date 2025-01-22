#version 460

in vec3 fragNorm;
in float fragType;

out vec4 finalColor;

uniform vec3 blockColors[5];
uniform vec3 lightDir;
uniform vec3 viewPos;

void main(){
    int typeIndex = int(fragType);
    if(typeIndex < 0 || typeIndex >= 5){
        finalColor = vec4(1.0, 0.0, 1.0, 1.0);
        return;
    }

    vec3 baseColor = blockColors[typeIndex];

    vec3 N = normalize(fragNorm);
    vec3 L = normalize(-lightDir);
    float diff = max(dot(N, L), 0.15);
    finalColor = vec4(baseColor * diff, 1.0);
}