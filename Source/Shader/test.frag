#version 430 core


layout(location=0) out vec4 color;

layout(std140) uniform uTest
{
	float delta;
	vec4 colors[10];
};

void main()
{
	color = vec4(1.0, 0.0, 0.0, 1.0);
}