#version 460 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 texCoord;
layout (location = 2) in int direction;

uniform mat4 camMatrix;

out vec2 aTexCoord;
out float Normal;

const float normals[] = float[](
    0.8f,  // North
    0.8f,  // South
    0.9f,  // West
    0.7f,  // East
    0.1f,  // Bottom
    1.0f   // Top
);

void main()
{
    // Pass texture coordinates
    aTexCoord = texCoord;

    // Ensure safe indexing
    Normal = normals[direction];

    // Apply camera transformation
    gl_Position = camMatrix * vec4(aPos, 1.0);
}

