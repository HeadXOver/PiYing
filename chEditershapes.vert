#version 330 core

layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec2 TexCoord;

uniform mat4 trc;

void main()
{
	gl_Position = trc * vec4(aPos.x, aPos.y, 0.f, 1.0f);
	TexCoord = vec2((aTexCoord.x + 1.0f) / 2, (aTexCoord.y + 1.0f) / 2);
}