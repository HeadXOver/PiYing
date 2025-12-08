#version 330 core

out vec4 FragColor;
uniform bool is_line = false;
uniform bool is_selected = false;

void main()
{
	FragColor = 
	is_line ? vec4(0.0f, 1.0f, 0.0f, 0.8f) : 
	is_selected ? vec4(0.0f, 0.0f, 1.0f, 0.15f) : 
	vec4(1.0f, 0.0f, 0.0f, 0.15f);
}