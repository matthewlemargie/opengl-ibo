#version 460 core

in vec2 aTexCoord;
out vec4 FragColor;
in float Normal;

uniform sampler2D tex0;

void main()
{
    FragColor = Normal * texture(tex0, aTexCoord);
    // FragColor = vec4(1.0f, 0.0f, 1.0f, 1.0f); 
}

