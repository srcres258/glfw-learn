#version 330 core

out vec4 FragColor;
in vec3 ourColor;

uniform float rOffset;
uniform float gOffset;
uniform float bOffset;

void main()
{
    FragColor = vec4((ourColor.r + rOffset) / 2, (ourColor.g + gOffset) / 2, (ourColor.b + bOffset) / 2, 1.0);
}
