#version 440

layout (location = 0) in vec3 vertex_position;
layout (location = 1) in vec3 vertex_color;
layout (location = 2) in vec3 vertex_normal;
layout (location = 3) in vec2 vertex_texcoord;
layout (location = 4) in float vertex_tex_id;

out vec3 vs_position;
out vec3 vs_color;
out vec3 vs_normal;
out vec2 vs_texcoord;
flat out float vs_tex_id;

out mat4 vs_transform;
out vec3 vs_to_light;
out vec3 vs_to_camera;

uniform mat4 transform_matrix;
uniform mat4 view_matrix;
uniform mat4 projection_matrix;
uniform vec3 light_position;

void main() {
    vec4 world_pos = (transform_matrix * vec4(vertex_position, 1.f));

    // Directly pass things to the fragment shader.
    vs_position = world_pos.xyz;
    vs_transform = transform_matrix;
    vs_texcoord = vertex_texcoord;
    vs_color = vertex_color;

    vs_tex_id = vertex_tex_id;


    gl_Position = projection_matrix * view_matrix * world_pos;
    
    vs_normal = (transform_matrix * vec4(vertex_normal, 0.f)).xyz;
    vs_to_light = light_position - world_pos.xyz;
    vs_to_camera = (inverse(view_matrix) * vec4(0.f, 0.f, 0.f, 1.f)).xyz - world_pos.xyz;
}