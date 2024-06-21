#version 330 core

in vec3 position;
// in vec3 color;
in vec2 texCoord;

// out vec3 m_color;
out vec2 m_texCoord;

uniform mat4 model;
uniform mat4 world;
uniform mat4 view;
uniform mat4 projection;

void main() {
    gl_Position = projection * view * world * model * vec4(position, 1.0);
    // m_color = color;
    m_texCoord = texCoord;
}