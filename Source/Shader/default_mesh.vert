#version 330 core
layout(location=0) in vec3 v_Position;

layout (std140) uniform UPrimitiveInfo
{
    mat4 modelMat;
}

layout (std140) uniform UCameraInfo
{
    mat4 viewMat;
    mat4 projectionMat;
    mat4 projectionViewMat;
};

void main()
{
    gl_Position = projectionViewMat * modelMat * vec4(v_Position, 1.0);
}