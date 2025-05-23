#version 330 core
layout (location = 0) in vec3 aPosition;
out vec3 TexCoords;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    TexCoords = aPosition;  // Skybox has no model matrix
    gl_Position = projection * view * vec4(aPosition, 1.0f);
}
