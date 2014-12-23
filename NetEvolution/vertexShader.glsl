#version 330 core

layout (location=0) in vec3 vertexPos;
layout (location=1) in vec3 vertexColor;
uniform mat4 transformMatrix;

out vec3 fragColor;

void main()
{
    gl_Position = transformMatrix * vec4(vertexPos, 1.0f);
    fragColor = vertexColor;
}