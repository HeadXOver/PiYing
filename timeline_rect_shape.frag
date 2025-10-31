#version 330 core

out vec4 FragColor;

uniform bool selected = false;

void main()
{
	FragColor = selected ? vec4(1.0f, 0.8f, 0.8f, 1.0f) : vec4(0.2f, 0.7f, 0.4f, 1.0f);
}