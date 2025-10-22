#version 330 core

out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D texture1;
uniform bool selected = false;

void main()
{
	FragColor = texture(texture1, TexCoord) * (selected ? vec4(1.0f, 0.8f, 0.8f, 1.0f) : vec4(1.0f, 1.0f, 1.0f, 1.0f));
}