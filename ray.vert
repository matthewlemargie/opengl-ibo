#version 460 core
layout(location = 0) in vec3 aPos;

uniform mat4 camMatrix;
uniform mat4 inverseView;

void main()
{
    gl_Position = camMatrix * inverseView * vec4(aPos, 1.0);
}