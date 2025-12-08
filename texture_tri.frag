#version 330 core

out vec4 FragColor;
uniform bool is_line = false;
uniform bool is_selected = false;

void main()
{
	vec3 full_color = is_selected ? vec3(0.0f, 0.0f, 1.0f) : vec3(1.0f, 0.0f, 0.0f);
	FragColor =  is_line ? vec4(0.0f, 1.0f, 0.0f, 0.8f) : vec4(full_color, 0.15f);
}