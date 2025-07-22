#version 430 core

layout(location=0) in vec3 v_pos;

uniform vec4 u_color;

void main()
{
	gl_Position = vec4(v_pos, 1.0);
}