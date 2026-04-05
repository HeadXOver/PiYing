#version 330 core

layout(location = 0) in vec2 aPos;

uniform mat4 trc;

uniform bool is_out;

void main()
{ 
	gl_Position = trc * vec4(aPos, 0.f, 1.f);
	gl_PointSize = is_out ? 14 : 10; 
}