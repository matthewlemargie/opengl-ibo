#version 460 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec4 col0;
layout (location = 3) in vec4 col1;
layout (location = 4) in vec4 col2;
layout (location = 5) in vec4 col3;

uniform mat4 camMatrix;
uniform vec3 lightPos;
//uniform mat4 model;

void main()
{
	mat4 instanceModel = mat4(col0, col1, col2, col3);
    mat4 translationMatrix = mat4(
        1.0, 0.0, 0.0, 0.0,
        0.0, 1.0, 0.0, 0.0,
        0.0, 0.0, 1.0, 0.0,
        lightPos.x, lightPos.y, lightPos.z, 1.0
    );
    float scale = 0.3f;
    vec4 worldPos = translationMatrix * instanceModel * vec4(scale * aPos, 1.0f);
    gl_Position = camMatrix * worldPos;
}
