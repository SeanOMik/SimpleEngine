#version 440

in vec3 vs_position;
in vec3 vs_color;
in vec2 vs_texcoord;

uniform bool texture_is_set;
uniform sampler2D vs_texture;

out vec4 fs_color;

void main() {
    if (texture_is_set) {
        fs_color = texture(vs_texture, vs_texcoord) * vec4(vs_color, 1.0);
    } else {
        fs_color = vec4(vs_color, 1.0);
    }
}