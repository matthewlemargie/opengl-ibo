#version 460 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec3 texCoord;
layout (location = 3) in ivec3 blockPosInWorld;

uniform mat4 camMatrix;

out vec3 fragNormal;
out vec3 atexCoord
out vec3 crntPos;

void main()
{
	mat4 instanceModel = mat4(
        1.0, 0.0, 0.0, float(blockPosInWorld.x),
        0.0, 1.0, 0.0, float(blockPosInWorld.y),
        0.0, 0.0, 1.0, float(blockPosInWorld.z),
        1.0, 0.0, 0.0, 0.0,
    );
    vec4 worldPos = instanceModel * vec4(aPos, 1.0f);
    crntPos = worldPos.xyz;

    mat3 normalMatrix = transpose(inverse(mat3(instanceModel)));
    fragNormal = normalize(normalMatrix * aNormal);
    aTexCoord = texCoord;

    gl_Position = camMatrix * worldPos;
}
