#version 330 core

layout(location=0) in vec3 v_Position;
layout(location=1) in vec3 v_Normal;

uniform mat4 v_modelMat;

layout (std140) uniform v_cameraInfo
{
    mat4 viewMat;
    mat4 projectionMat;
    mat4 projectionViewMat;
};

out vec3 v_NormalOut;

void main()
{
    gl_Position = projectionViewMat * v_modelMat * vec4(v_Position, 1.0);
    v_NormalOut = mat3(transpose(inverse(v_modelMat))) * v_Normal; // Transform normal to world space
}