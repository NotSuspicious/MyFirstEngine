#version 330 core

out vec4 FragColor;

in vec3 m_color;
in vec2 m_texCoord;

uniform sampler2D m_Texture1;
uniform sampler2D m_Texture2;

void main(){
    FragColor = mix(
        texture(m_Texture1, m_texCoord),
        texture(m_Texture2, m_texCoord),
        0.2
        );
}