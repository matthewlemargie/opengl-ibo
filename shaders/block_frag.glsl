#version 460 core

in vec2 aTexCoord;
out vec4 FragColor;

uniform sampler2D tex0;

void main()
{
    FragColor = texture(tex0, aTexCoord);
    // FragColor = vec4(1.0f, 0.0f, 1.0f, 1.0f); 
}

