#version 440

layout (location = 0) in vec3 vertex_position;
layout (location = 1) in vec3 vertex_color;
layout (location = 2) in vec2 vertex_texcoord;

out vec3 vs_position;
out vec3 vs_color;
out vec2 vs_texcoord;
out mat4 vs_transform;

uniform mat4 transform_matrix;
uniform mat4 view_matrix;
uniform mat4 projection_matrix;

void main() {
    vs_position = vertex_position;
    vs_transform = transform_matrix;
    vs_color = vertex_color;
    vs_texcoord = vertex_texcoord;

    gl_Position = projection_matrix * view_matrix * transform_matrix * vec4(vertex_position, 1.0f);
    
    /* vs_position = vec4(transform_matrix * vec4(vertex_position, 1.f)).xyz;
    vs_transform = transform_matrix;
    vs_color = vertex_color;
    vs_texcoord = vertex_texcoord;

    gl_Position = projection_matrix * view_matrix * transform_matrix * vec4(vertex_position, 1.0f); */
}