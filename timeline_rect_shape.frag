#version 330 core

out vec4 FragColor;
in vec2 TexCoord;

uniform sampler2D texture1;
uniform bool selected = false;

void main()
{
	FragColor = selected ? texture(texture1, TexCoord) * vec4(0.6f, 0.6, 1.f, 1.0f) : texture(texture1, TexCoord) * vec4(1.f, 1.f, 1.f, 1.0f);
}