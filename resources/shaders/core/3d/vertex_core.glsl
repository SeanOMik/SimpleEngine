#version 440

layout (location = 0) in vec3 vertex_position;
layout (location = 1) in vec3 vertex_color;
layout (location = 2) in vec3 vertex_normal;
layout (location = 3) in vec2 vertex_texcoord;
// TODO: Make these optional
layout (location = 4) in vec3 vertex_tangent;
layout (location = 5) in vec3 vertex_bitangent;

out vec3 vs_position;
out vec3 vs_color;
out vec3 vs_normal;
out vec3 vs_world_normal; // normal in world space
out vec2 vs_texcoord;

out mat4 vs_transform;

out vec3 vs_view_pos;
out vec3 vs_light_pos;
out vec3 vs_frag_pos;

out mat3 vs_tbn;

out vec3 vs_tangent;
out vec3 vs_bitangent;

uniform mat4 u_transform_matrix;
uniform mat4 u_view_matrix;
uniform mat4 u_projection_matrix;

uniform vec3 u_view_pos;
uniform vec3 u_light_position;

void main() {
    vec4 world_pos = (u_transform_matrix * vec4(vertex_position, 1.f));

    // Directly pass things to the fragment shader.
    vs_position = world_pos.xyz;
    vs_transform = u_transform_matrix;
    vs_texcoord = vertex_texcoord;
    vs_color = vertex_color;

    gl_Position = u_projection_matrix * u_view_matrix * world_pos;
    
    vs_normal = (u_transform_matrix * vec4(vertex_normal, 0.f)).xyz;
    
    vs_view_pos = u_view_pos;
    vs_light_pos = u_light_position;
    vs_frag_pos = vec3(u_transform_matrix * vec4(vertex_position, 1.f));
    vs_world_normal = mat3(transpose(inverse(u_transform_matrix))) * vertex_normal; // TODO: Do this calculation on the CPU then send to GPU via a uniform

    // Calculate the TBN (Tangent, Bi-tangent, and normal matrix)
    vec3 T = normalize(vec3(u_transform_matrix * vec4(vertex_tangent, 1.f)));
    vec3 B = normalize(vec3(u_transform_matrix * vec4(vertex_bitangent, 1.f)));
    vec3 N = normalize(vec3(u_transform_matrix * vec4(vertex_normal, 1.f)));
    vs_tbn = mat3(T, B, N);

    vs_tangent = vertex_tangent;
    vs_bitangent = vertex_bitangent;
}