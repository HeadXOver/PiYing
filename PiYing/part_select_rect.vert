#version 330 core

layout(location = 0) in vec2 aPos;

uniform vec2 trans;
uniform float ratio;

void main()
{
    vec2 pos = vec2(aPos.x, (aPos.y - 1.f) * ratio + 1.f) * 0.2f;
    pos = pos + trans;
    pos = vec2(pos.x * 0.98f - 0.02f, pos.y * 0.98f + 0.02f); // 留出滑块的空间
    gl_Position = vec4(pos, 0.0f, 1.0f);
}