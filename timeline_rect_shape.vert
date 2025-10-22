#version 330 core

layout (location = 0) in vec2 aPos;

out vec2 TexCoord;

uniform vec4 rect;

void main()
{
	gl_Position = vec4(rect.x + aPos.x * rect.z, rect.y + aPos.y * rect.w, 0.f, 1.0f);
	TexCoord = vec2((aPos.x + 1.0f) / 2 / rect.z, (aPos.y + 1.0f) / 2);
}