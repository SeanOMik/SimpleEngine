#version 440

in vec3 vs_position;
in mat4 vs_transform;
in vec2 vs_texcoord;
in vec3 vs_normal;
in vec3 vs_to_light;
in vec3 vs_to_camera;

uniform bool texture_is_set;
uniform sampler2D vs_texture;
uniform vec3 light_color;
uniform float shine_damper;
uniform float reflectivity;

out vec4 fs_color;

void main() {
    // Lighting
    vec3 unit_normal = normalize(vs_normal);
    vec3 unit_light_vector = normalize(vs_to_light);

    float dot_prod = dot(unit_normal, unit_light_vector);
    float brightness = max(dot_prod, 0.f);
    vec3 diffuse = brightness * light_color;

    // Specular lighting
    // only do all this math is reflectivity is > 0
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

    if (texture_is_set) {
        //fs_color = vec4(0.5 * unit_normal + vec3(0.5), 1.f); // Visualize normals
        fs_color = vec4(diffuse, 1.f) * texture(vs_texture, vs_texcoord) + vec4(final_specular, 1.f);
    } else {
        fs_color = vec4(diffuse, 1.f) + vec4(final_specular, 1.f);
    }
}