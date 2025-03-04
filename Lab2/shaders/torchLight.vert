#version 410 core

layout(location=0) in vec3 vPosition;
layout(location=1) in vec3 vNormal;

out vec3 fNormal;
out vec4 fPosEye;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat3 normalMatrix;

void main()
{
    fPosEye = view * model * vec4(vPosition, 1.0f);
    fNormal = normalize(normalMatrix * vNormal);
    gl_Position = projection * fPosEye;
}
