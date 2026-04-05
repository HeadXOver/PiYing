#version 330 core

layout (location = 0) in vec2 aPos;

out vec2 TexCoord;

uniform vec4 trans;
uniform float lenth;

void main()
{
	gl_Position = vec4(aPos.x * trans.x + trans.z, aPos.y * trans.y + trans.w, 0.f, 1.0f);
	TexCoord = vec2((aPos.x + 1.0f) / 2.f * lenth, (aPos.y + 1.0f) / 2.f);
}