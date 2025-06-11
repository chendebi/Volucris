#version 330 core
layout(location=0) out vec4 color;

layout (std140) uniform UDirectionLight
{
    vec4 color;
    vec4 direction;
    float intensity;
} directionLight;

uniform vec3 fcolor;

uniform sampler2D colorTex;

in vec3 uv;

void main()
{
    color = texture(colorTex, uv.xy);
}