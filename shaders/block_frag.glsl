#version 460 core

in vec3 fragNormal;
in vec3 crntPos;
out vec4 FragColor;

// uniform vec4 lightColor;
// uniform vec3 lightPos;

uniform sampler2D tex0;

void main()
{
    vec3 normal = normalize(fragNormal);
    vec3 lightDirection = normalize(lightPos - crntPos);
    float dist = distance(lightPos, crntPos);

    // Calculate diffuse lighting
    float diffuse = max(dot(normal, lightDirection), 0.0f);

    // Set the fragment color
    FragColor = texture(tex0, aTexCoord);
    // FragColor = vec4(1.0f, 0.0f, 1.0f, 1.0f);
}
