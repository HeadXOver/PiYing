#version 330 core

out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D texture1;
uniform vec4 aColor = vec4(1.f, 1.f, 1.f, 1.f);

void main()
{
// 检查纹理坐标是否在有效范围内
    if(TexCoord.x < 0.0 || TexCoord.x > 1.0 || 
       TexCoord.y < 0.0 || TexCoord.y > 1.0) {
        FragColor = vec4(0.0, 0.0, 0.0, 0.0); // 透明
        return;
    }
    
	FragColor = texture(texture1, TexCoord) * aColor;
}