#version 330 core
layout(location=0) out vec4 color;

layout (std140) uniform UDirectionLight
{
    vec4 color;
    vec4 direction;
    float intensity;
} directionLight;

uniform vec3 fcolor;

void main()
{
    color = directionLight.color;
}