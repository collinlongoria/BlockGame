#version 460

in vec3 fragNorm;
in float fragType;

out vec4 finalColor;

uniform vec3 blockColors[5];
uniform vec3 viewPos;

void main(){
    int typeIndex = int(fragType);
    if(typeIndex < 0 || typeIndex >= 5){
        finalColor = vec4(1.0, 0.0, 1.0, 1.0);
        return;
    }
    else if(typeIndex == 1){
        finalColor = vec4(1.0, 0.0, 0.0, 1.0);
    }
    else if(typeIndex == 2){
        finalColor = vec4(0.0, 1.0, 0.0, 1.0);
    }
    else{
        finalColor = vec4(0.8, 0.8, 0.8, 1.0);
    }
}