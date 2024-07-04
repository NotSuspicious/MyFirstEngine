#version 330 core

out vec4 FragColor;

// in vec3 m_color;
in vec2 m_texCoord;

uniform sampler2D m_Texture1;
uniform sampler2D m_Texture2;

uniform float m_xVertexOffset = 0.0f;
uniform float m_yVertexOffset = 0.0f;

void main(){
    vec2 modifiedTexCoord = m_texCoord;
    modifiedTexCoord.x += m_xVertexOffset;
    modifiedTexCoord.y += m_yVertexOffset;
    FragColor = mix(
        texture(m_Texture1, modifiedTexCoord),
        texture(m_Texture2, modifiedTexCoord),
        0.7f
        );
}