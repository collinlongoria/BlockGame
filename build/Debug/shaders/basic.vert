#version 460

layout (location = 0) in vec3 inPos;
layout (location = 1) in vec3 inNormal;
layout (location = 2) in vec2 inUV;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;

out vec3 normal;
out vec3 fragPos;

void main(){
    fragPos = vec3(uModel * vec4(inPos, 1.0));

    mat3 normalMatrix = transpose(inverse(mat3(uModel)));
    normal = normalize(normalMatrix * inNormal);

    gl_Position = uProjection * uView * uModel * vec4(inPos, 1.0);
}