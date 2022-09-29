#version 440

in vec3 vs_position;
in vec3 vs_color;
in vec3 vs_normal;
in vec3 vs_world_normal; // Normal in world space
in vec2 vs_texcoord;

in mat4 vs_transform;

in vec3 vs_view_pos;
in vec3 vs_light_pos;
in vec3 vs_frag_pos;

const int SAMP_DIFFUSE = 0;
const int SAMP_SPECULAR = 1;

struct Material {
    sampler2D diffuse;

    bool has_specular_map;
    sampler2D specular_map;

    float ambient_strength;
    float diffuse_strength;
    float specular_strength;
    
    float shine_factor;
};

uniform Material u_material;

uniform vec3 u_light_color;

out vec4 fs_color;

vec3 calculate_lighting();

void main() {
    
    
    // Combine diffuse lighting, specular, and the texture into one color.
    //fs_color = vec4(diffuse, 1.f) * texture(u_material.diffuse, vs_texcoord) + vec4(final_specular, 1.f);

    vec3 lighting = calculate_lighting();

    fs_color = vec4(lighting, 1.f) * texture(u_material.diffuse, vs_texcoord);
}

vec3 calculate_lighting() {
    // Ambient
    //float ambient_strength = 0.1;
    vec3 ambient = u_material.ambient_strength * u_light_color;

    // Diffuse
    vec3 norm = normalize(vs_world_normal);
    vec3 light_dir = normalize(vs_light_pos - vs_frag_pos);
    float diff = max(dot(norm, light_dir), 0.f);
    vec3 diffuse = (diff * u_material.diffuse_strength) * u_light_color;

    // Specular
    float specular_strength = 0.5;
    vec3 view_dir = normalize(vs_view_pos - vs_frag_pos);
    vec3 reflect_dir = reflect(-light_dir, norm);
    float spec = pow(max(dot(view_dir, reflect_dir), -0.f), 32 * u_material.shine_factor);
    vec3 specular = specular_strength * (spec * u_material.specular_strength) * u_light_color;

    // Check if the specular map is set before trying to set it
    if (u_material.has_specular_map) {
        specular = specular * texture(u_material.specular_map, vs_texcoord).r;
    }

    return ambient + diffuse + specular;
}