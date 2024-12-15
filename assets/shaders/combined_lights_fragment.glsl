#version 460 core

// ===================================== io vars =========================================
in vec3 normal;
in vec3 frag_pos;
in vec2 texture_coordinates;

out vec4 frag_color;

// ===================================== structures ======================================
struct Material {
    sampler2D diffuse_texture;
    sampler2D specular_texture;

    float shininess; // specular coefficient
};

struct Directional_Light {
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct Point_Light {
    vec3 pos;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    // attenuation equation coefficients
    // float constant is always 1.0f
    float linear;
    float quadratic;
};

struct Spot_Light {
    vec3 pos;
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    // cosines of inner and outer cutoff angle of the spotlight cone
    float inner_cosine_angle;
    float outer_cosine_angle;
};

// ===================================== uniforms ========================================
uniform Material u_material;
uniform Directional_Light u_directional_light;
uniform Point_Light u_point_lights[2];
uniform Spot_Light u_spot_light;

uniform vec3 u_view_pos; // position from viewer (camera pos in world space)

// ===================================== functions =======================================
vec3 calculate_directional_light()
{
    vec3 ambient_light = u_directional_light.ambient * vec3(texture(u_material.diffuse_texture, texture_coordinates));

    vec3 light_direction = normalize(-u_directional_light.direction);
    vec3 norm = normalize(normal); // normal from surface

    float diffuse_intensity = max(dot(light_direction, normal), 0.0f);
    vec3 diffuse_light = u_directional_light.diffuse * diffuse_intensity * vec3(texture(u_material.diffuse_texture, texture_coordinates));

    // negate light_direction to get vector from light source to object surface,
    // then reflect it on surface normal to get reflection from surface to camera
    vec3 refl = reflect(-light_direction, norm);
    vec3 view_dir = normalize(u_view_pos-frag_pos);
    float specular_intensity = pow(max(dot(refl, view_dir), 0.0f), u_material.shininess);
    vec3 specular_light = u_directional_light.specular * specular_intensity * vec3(texture(u_material.specular_texture, texture_coordinates));

    return vec3(ambient_light + diffuse_light + specular_light);
}

vec3 calculate_point_light(Point_Light point_light)
{
    float d = length(point_light.pos-frag_pos);
    float attenuation = 1.0f / (1.0f + point_light.linear*d + point_light.quadratic*d*d);

    vec3 ambient_light = point_light.ambient * vec3(texture(u_material.diffuse_texture, texture_coordinates)) * attenuation;

    vec3 light_direction = normalize(point_light.pos-frag_pos); // from pixel to light_source
    vec3 norm = normalize(normal); // normal from surface
    float diffuse_intensity = max(dot(light_direction, normal), 0.0f);
    vec3 diffuse_light = point_light.diffuse * diffuse_intensity * vec3(texture(u_material.diffuse_texture, texture_coordinates)) * attenuation;

    vec3 refl = reflect(-light_direction, norm);
    vec3 view_dir = normalize(u_view_pos-frag_pos);
    float specular_intensity = pow(max(dot(refl, view_dir), 0.0f), u_material.shininess);
    vec3 specular_light = point_light.specular * specular_intensity * vec3(texture(u_material.specular_texture, texture_coordinates)) * attenuation;

    return vec3(ambient_light + diffuse_light + specular_light);
}

void main()
{
    vec3 final_color = vec3(0.0f);
    
    final_color += calculate_directional_light();
    for (int i=0; i<2; ++i) {
        final_color += calculate_point_light(u_point_lights[i]);
    }
    
    frag_color = vec4(final_color, 1.0f);
}
