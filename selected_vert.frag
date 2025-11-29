#version 330 core

out vec4 fragColor;

uniform bool is_out;

void main()
{ 
	fragColor = is_out ? vec4(0, 0, 0, 1) : vec4(1, 0, 0, 1); 
}