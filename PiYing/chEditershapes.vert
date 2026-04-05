#version 330 core

layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec2 TexCoord;

uniform mat4 trc;
uniform float prescale = 1.f;

void main()
{
	gl_Position = trc * vec4(aPos.x, aPos.y, 0.f, 1.0f);
	TexCoord = (vec2(aTexCoord.x, aTexCoord.y / prescale) + vec2(1.f, 1.f)) * 0.5f;
}