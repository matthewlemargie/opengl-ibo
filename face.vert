#version 460 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 texCoord;
layout (location = 3) in vec4 col0;
layout (location = 4) in vec4 col1;
layout (location = 5) in vec4 col2;
layout (location = 6) in vec4 col3;

uniform mat4 camMatrix;
//uniform mat4 model;

out vec3 fragNormal;
out vec3 crntPos;
out vec2 aTexCoord;

void main()
{
	mat4 instanceModel = mat4(col0, col1, col2, col3);
    vec4 worldPos = instanceModel * vec4(10.0f * aPos, 1.0f);
    crntPos = worldPos.xyz;

    mat3 normalMatrix = transpose(inverse(mat3(instanceModel)));
    fragNormal = normalize(normalMatrix * aNormal);

    aTexCoord = texCoord;

    gl_Position = camMatrix * worldPos;
}
