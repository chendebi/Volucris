#version 330 core

in vec3 v_NormalOut;
layout (std140) uniform v_directionLight
{
    vec4 color;
    vec4 direction;
    float intensity;
} directionLight;

layout(location=0) out vec4 FragColor;

void main()
{
	FragColor.xyz = directionLight.color.xyz;
}