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

//in mat3 vs_tbn;

in vec3 vs_tangent_light_pos;
in vec3 vs_tangent_view_pos;
in vec3 vs_tangent_frag_pos;

const int SAMP_DIFFUSE = 0;
const int SAMP_SPECULAR = 1;

struct Material {
    sampler2D diffuse;

    bool has_specular_map;
    sampler2D specular_map;

    bool has_normal_map;
    sampler2D normal_map;

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
    // Skip completely transparent fragments.
    /* vec4 diffuse = texture(u_material.diffuse, vs_texcoord);
    if (diffuse.a < 0.1) {
        discard;
    } */

    vec3 lighting = calculate_lighting();

    fs_color = vec4(lighting, 1.f) * texture(u_material.diffuse, vs_texcoord);
}

vec3 calculate_lighting() {
    vec3 normal = vec3(0.f, 0.f, 1.f); // default tangent-space normal
    
    // Check if the normal map is set before trying to apply it.
    if (u_material.has_normal_map) {
        normal = texture(u_material.normal_map, vs_texcoord).rgb;

        // transform normal vector to range [-1,1]
        normal = normalize(normal * 2.f - 1.f);  // this normal is in tangent space
    }
   
    // Get diffuse color
    vec3 diffuse_map = texture(u_material.diffuse, vs_texcoord).rgb;
    
    // Ambient lighting
    vec3 ambient = 0.1f * u_material.ambient_strength * diffuse_map;
    
    // Diffuse lighting
    vec3 light_dir = normalize(vs_tangent_light_pos - vs_tangent_frag_pos);
    float diff = max(dot(light_dir, normal), 0.f);
    vec3 diffuse = diff * u_material.diffuse_strength * diffuse_map;
    
    // Specular lighting
    vec3 view_dir = normalize(vs_tangent_view_pos - vs_tangent_frag_pos);
    vec3 reflect_dir = reflect(-light_dir, normal);
    vec3 halfway_dir = normalize(light_dir + view_dir);  
    float spec = pow(max(dot(normal, halfway_dir), 0.f), 32.f * u_material.shine_factor);
    vec3 specular = vec3(0.2f * u_material.specular_strength) * spec;
    
    // Check if the specular map is set before trying to apply it.
    if (u_material.has_specular_map) {
        specular = specular * texture(u_material.specular_map, vs_texcoord).r;
    }

    return ambient + diffuse + specular;
}