#version 330 core
layout(location=0) out vec4 color;

uniform vec3 fcolor;

void main()
{
    color = vec4(fcolor, 1.0);
}