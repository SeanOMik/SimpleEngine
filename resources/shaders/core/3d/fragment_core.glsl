#version 440

in vec3 vs_position;
in vec3 vs_color;
in vec3 vs_normal;
in vec2 vs_texcoord;
flat in float vs_tex_id; // < 0 is reserved for solid colored objects.

in mat4 vs_transform;
in vec3 vs_to_light;
in vec3 vs_to_camera;

uniform sampler2D u_textures[16];
uniform float u_texture_shine[16];
uniform float u_texture_reflectivity[16];

uniform vec3 light_color;

out vec4 fs_color;

vec3 calculate_specular(vec3 unit_normal, float shine_damper, float reflectivity);

void main() {
    // Lighting
    vec3 unit_normal = normalize(vs_normal);
    vec3 unit_light_vector = normalize(vs_to_light);

    float dot_prod = dot(unit_normal, unit_light_vector);
    float brightness = max(dot_prod, 0.f);
    vec3 diffuse = brightness * light_color;

    if (vs_tex_id > -1) {
        int id = int(vs_tex_id);

        float shine_damper = u_texture_shine[id];
        float reflectivity = u_texture_reflectivity[id];
        vec3 final_specular = calculate_specular(unit_normal, shine_damper, reflectivity);
        
        fs_color = vec4(diffuse, 1.f) * texture(u_textures[id], vs_texcoord) + vec4(final_specular, 1.f);
    } else {
        fs_color = vec4(vs_color, 1.f); // We don't add any reflectivity to solid colored vectors.
    }
}

vec3 calculate_specular(vec3 unit_normal, float shine_damper, float reflectivity) {
    vec3 final_specular = vec3(0.f);
    if (reflectivity > 0) {
        vec3 unit_vector_to_camera = normalize(vs_to_camera);
        vec3 light_direction = -unit_vector_to_camera;
        vec3 reflected_light_dir = reflect(light_direction, unit_normal);
        float specular_factor = dot(reflected_light_dir, unit_vector_to_camera);
        specular_factor = max(specular_factor, 0.f);
        float damped_specular = pow(specular_factor, shine_damper);
        final_specular = damped_specular * reflectivity * light_color;
    }

    return final_specular;
}