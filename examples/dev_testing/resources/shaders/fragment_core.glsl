#version 440

in vec3 vs_position;
in mat4 vs_transform;
in vec2 vs_texcoord;
in vec3 vs_normal;
in vec3 vs_to_light;

uniform bool texture_is_set;
uniform sampler2D vs_texture;
uniform vec3 light_color;

out vec4 fs_color;

void main() {
    vec3 unit_normal = normalize(vs_normal);
    vec3 unit_light_vector = normalize(vs_to_light);

    float dot_prod = dot(unit_normal, unit_light_vector);
    float brightness = max(dot_prod, 0.f);
    vec3 diffuse = brightness * light_color;

    if (texture_is_set) {
        fs_color = vec4(diffuse, 1.f) * texture(vs_texture, vs_texcoord);
    } else {
        fs_color = vec4(diffuse, 1.f);
    }
}