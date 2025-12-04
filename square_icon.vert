#version 330 core

layout (location = 0) in vec2 aPos;

out vec2 TexCoord;

uniform vec2 dPos = vec2(0.f, 0.f);
uniform float size = 2.f;

uniform vec2 texCenter = vec2(0.f, 0.f);
uniform float texSize = 2.f;

void main()
{
	float halfSize = size / 2.f;

	gl_Position = vec4(aPos.x * halfSize - (1.f - halfSize), aPos.y * halfSize + (1.f - halfSize), 0.f, 1.0f);
	vec2 texCoord = vec2(aPos.x * texSize / 2.f, aPos.y * texSize / 2.f);
	TexCoord = (texCoord + vec2(1.f, 1.f)) / 2.f;
}