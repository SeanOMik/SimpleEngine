#version 440

in vec3 vs_position;
in vec3 vs_color;
in vec3 vs_normal;
in vec2 vs_texcoord;

in mat4 vs_transform;
in vec3 vs_to_light;
in vec3 vs_to_camera;

const int SAMP_DIFFUSE = 0;
uniform sampler2D u_samplers[16];
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

    // Calculate the specular
    float shine_damper = u_texture_shine[SAMP_DIFFUSE];
    float reflectivity = u_texture_reflectivity[SAMP_DIFFUSE];
    vec3 final_specular = calculate_specular(unit_normal, shine_damper, reflectivity);
    
    // Combine diffuse lighting, specular, and the texture into one color.
    fs_color = vec4(diffuse, 1.f) * texture(u_samplers[SAMP_DIFFUSE], vs_texcoord) + vec4(final_specular, 1.f);
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