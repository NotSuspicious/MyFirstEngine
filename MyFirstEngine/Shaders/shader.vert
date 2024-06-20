#version 330 core

in vec3 position;
in vec3 color;
in vec2 texCoord;

out vec3 m_color;
out vec2 m_texCoord;

void main() {
    m_color = color;
    m_texCoord = texCoord;
    gl_Position = vec4(position, 1.0);
}