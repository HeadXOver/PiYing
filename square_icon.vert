#version 330 core

layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec2 TexCoord;

uniform float scale = 1.f;
uniform float ratio = 1.f;
uniform float scroll = 0.f;
uniform float x = 0.f;
uniform float y = 0.f;

void main()
{
	vec2 pos2d = vec2(aPos.x * scale + x, (aPos.y * scale + y - 1.f) * ratio + 1.f);
	pos2d = vec2(pos2d.x * 0.98f - 0.02f, pos2d.y * 0.98f + scroll); // 留出滑块和上栏的空间
	gl_Position = vec4(pos2d, 0.f, 1.0f);
	TexCoord = (aTexCoord + vec2(1.f, 1.f)) * 0.5f;
}