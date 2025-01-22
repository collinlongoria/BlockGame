#version 460

layout(location = 0) in vec3 inPos;
layout(location = 1) in vec3 inNorm;
layout(location = 2) in vec2 inUV;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProj;

out vec3 fragNorm;
out float fragType;

void main(){
    fragNorm = inNorm;
    fragType = inUV.x;

    gl_Position = uProj * uView * uModel * vec4(inPos, 1.0);
}
