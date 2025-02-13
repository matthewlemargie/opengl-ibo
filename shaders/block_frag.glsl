#version 460 core

in vec3 fragNormal;
in vec2 aTexCoord;  // Declare aTexCoord as input from vertex shader
out vec4 FragColor;

uniform sampler2D tex0;

void main()
{
    // Calculate lighting (optional, you can add your own logic)
    vec3 normal = normalize(fragNormal);
    // Uncomment below lines to add lighting logic
    // vec3 lightDirection = normalize(lightPos - crntPos);
    // float diffuse = max(dot(normal, lightDirection), 0.0f);

    // Sample the texture and apply it to the fragment
    FragColor = texture(tex0, aTexCoord);
    // Uncomment this line for a hardcoded color:
    // FragColor = vec4(1.0f, 0.0f, 1.0f, 1.0f); 
}

