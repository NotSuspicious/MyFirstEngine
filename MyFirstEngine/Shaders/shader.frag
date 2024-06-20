#version 330 core

out vec4 FragColor;

in vec3 m_color;
in vec2 m_texCoord;

uniform sampler2D m_Texture;

void main(){
    FragColor = texture(m_Texture, m_texCoord);
}