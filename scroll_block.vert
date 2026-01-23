#version 330 core

layout(location = 0) in vec2 aPos;

uniform float scale = 1.f;
uniform float trans = 0.f;

void main()
{ 
	vec2 pos = vec2(aPos.x, (aPos.y - 1) / scale + 1.f - trans);
	gl_Position = vec4(pos, 0.f, 1.f);
}