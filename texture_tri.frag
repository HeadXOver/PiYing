#version 330 core

out vec4 FragColor;
uniform bool is_line;

void main()
{
	FragColor = is_line ? vec4(0.0f, 1.0f, 0.0f, 1.f) : vec4(1.0f, 0.0f, 0.0f, 0.2f);
}