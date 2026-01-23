#version 330 core

layout(location = 0) in vec2 aPos;

uniform vec2 trans;
uniform float ratio = 1.f;

void main()
{ 
	vec2 pos = vec2(aPos.x, aPos.y * ratio);
	gl_Position = vec4(aPos + trans, 0.f, 1.f);
}