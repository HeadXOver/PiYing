#version 330 core

layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec2 TexCoord;
uniform float scale = 1.f;
uniform float x = 0.f;
uniform float y = 0.f;

void main()
{
	gl_Position = vec4(aPos.x * scale + x, aPos.y * scale + y, 0.f, 1.0f);
	TexCoord = (aTexCoord + vec2(1.f, 1.f)) / 2.f;
}