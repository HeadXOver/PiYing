#version 330 core

layout (location = 0) in vec2 aPos;
uniform vec4 trans;

void main()
{
	gl_Position = vec4(aPos.x * trans.x + trans.z, aPos.y * trans.y + trans.w, 0.f, 1.0f);
}