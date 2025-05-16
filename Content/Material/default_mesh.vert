#version 330 core
layout(location=0) in vec3 v_Position;

uniform mat4 v_modelMat;

layout (std140) uniform v_cameraInfo
{
    mat4 viewMat;
    mat4 projectionMat;
    mat4 projectionViewMat;
};

void main()
{
    gl_Position = projectionViewMat * v_modelMat * vec4(v_Position, 1.0);
}