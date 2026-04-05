#version 330 core

layout (location = 0) in vec2 aPos;

out vec2 TexCoord;

uniform mat4 trc;

void main()
{
	gl_Position = trc * vec4(aPos, 0.f, 1.0f);
	TexCoord = (aPos + vec2(1.f, 1.f)) * 0.5f;
}