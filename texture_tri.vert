#version 330 core

layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aPos_s;

uniform mat4 trc;
uniform bool is_skelen;

void main()
{
	gl_Position = trc * (is_skelen ? vec4(aPos_s.x, aPos_s.y, 0.f, 1.0f) : vec4(aPos.x, aPos.y, 0.f, 1.0f));
}