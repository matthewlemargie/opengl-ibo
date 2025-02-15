#version 460 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 texCoord;

uniform mat4 camMatrix;

out vec2 aTexCoord;

void main()
{
    // Pass texture coordinates to fragment shader
    aTexCoord = texCoord;  // Pass texture coordinates to fragment shader

    // Apply camera transformation
    gl_Position = camMatrix * vec4(aPos, 1.0f);
}

