#version 330 core

layout (location = 0) in vec2 aPos;

void main()
{
	gl_Position = vec4(aPos.x / 2.f, aPos.y / 2.f, 0.f, 1.0f);
}