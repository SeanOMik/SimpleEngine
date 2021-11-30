#version 440

layout (location = 0) in vec3 vertex_position;
layout (location = 1) in vec3 vertex_color;
layout (location = 2) in vec2 vertex_texcoord;

out vec3 vs_position;
out vec3 vs_color;
out vec2 vs_texcoord;
out mat4 vs_transform;

uniform mat4 transform;

void main() {
    vs_position = vertex_position;
    vs_transform = transform;
    vs_color = vertex_color;
    vs_texcoord = vertex_texcoord;

    gl_Position = transform * vec4(vertex_position, 1.0f);
}