#version 460 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 texCoord;
layout (location = 3) in vec3 blockPosInWorld;

uniform mat4 camMatrix;

out vec3 fragNormal;
out vec2 aTexCoord;  // Declare aTexCoord correctly
out vec3 crntPos;

void main()
{
    // Create instance model transformation matrix
    mat4 instanceModel = mat4(1.0);  // Initialize identity matrix
    instanceModel[3] = vec4(float(blockPosInWorld.x), float(blockPosInWorld.y), float(blockPosInWorld.z), 1.0);  // Corrected translation

    // Calculate world position
    vec4 worldPos = instanceModel * vec4(aPos, 1.0f);
    crntPos = worldPos.xyz;

    // Apply normal transformation for lighting calculation
    mat3 normalMatrix = transpose(inverse(mat3(instanceModel)));
    fragNormal = normalize(normalMatrix * aNormal);

    // Pass texture coordinates to fragment shader
    aTexCoord = texCoord;  // Pass texture coordinates to fragment shader

    // Apply camera transformation
    gl_Position = camMatrix * worldPos;
}

