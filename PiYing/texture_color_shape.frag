#version 330 core

out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D texture1;
uniform vec4 aColor = vec4(1.f, 1.f, 1.f, 1.f);

void main()
{
	FragColor = texture(texture1, TexCoord) * aColor;
}