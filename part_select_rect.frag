#version 330 core

out vec4 FragColor;

uniform bool moving = false;

void main()
{
    FragColor = moving ? vec4(0.3f, 0.3f, 0.6f, 1.0f) : vec4(0.6f, 0.3f, 0.3f, 1.0f);
}