#version 410 core


out vec4 fColor;

uniform vec3 rainColor;

void main() {
    fColor = vec4(rainColor, 0.7);
}