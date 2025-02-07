#version 460 core

in vec3 fragNormal;
in vec3 crntPos;
in vec2 aTexCoord;
out vec4 FragColor;

uniform sampler2D tex0;

void main()
{
    FragColor = texture(tex0, aTexCoord);
}
