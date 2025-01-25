#version 460 core

// ===================================== io vars =========================================
in vec3 normal;
in vec3 frag_pos;
in vec2 texture_coordinates;

out vec4 frag_color;

// ===================================== structures ======================================

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

struct Lighting_Values {
    vec3 norm;
    vec3 sampled_diffuse_texture;
    vec3 sampled_specular_texture;
    vec3 view_dir;
} lighting_values;

// ===================================== uniforms ========================================
uniform Point_Light u_point_lights[16]; // Max number of point lights

uniform float u_shininess; // specular coefficient

uniform vec3 u_view_pos; // position from viewer (camera pos in world space)
uniform int u_point_lights_count;
uniform bool u_blinn;
uniform bool u_do_point_light;
uniform bool u_do_gamma_correction;
uniform bool u_do_attenuation;
uniform bool u_reverse_gamma;
uniform bool u_do_linear;
uniform sampler2D u_diffuse_texture;

vec3 calculate_blinn_phong_light(Point_Light point_light)
{
    float d = length(point_light.pos-frag_pos);
    float attenuation = 0.0f;
    
    if (u_do_attenuation) {
        if (u_do_linear) {
            attenuation = 1.0f / d;
        }
        else {
            attenuation = 1.0f / (d*d);
        }
    }
    else {
        attenuation = 1.0f / (1.0f + point_light.linear*d + point_light.quadratic*d*d);
    }

    vec3 ambient_light = point_light.ambient * lighting_values.sampled_diffuse_texture * attenuation;

    vec3 light_direction = normalize(point_light.pos-frag_pos); // from pixel to light_source
    float diffuse_intensity = max(dot(light_direction, lighting_values.norm), 0.0f);
    vec3 diffuse_light = point_light.diffuse * diffuse_intensity * lighting_values.sampled_diffuse_texture * attenuation;

    float specular_intensity = 1.0f;
    if (u_blinn) {
        vec3 halfway_dir = normalize(light_direction+lighting_values.view_dir);
        specular_intensity = pow(max(dot(lighting_values.norm, halfway_dir), 0.0f), u_shininess);
    } 
    else {
        vec3 refl = reflect(-light_direction, lighting_values.norm);
        specular_intensity = pow(max(dot(refl, lighting_values.view_dir), 0.0f), u_shininess);
    }
    vec3 specular_light = point_light.specular * specular_intensity * lighting_values.sampled_specular_texture * attenuation;

    return vec3(ambient_light + diffuse_light + specular_light);
}

void main()
{
    // calculate shared lighting values once
    lighting_values.norm = normalize(normal);
    if (u_reverse_gamma) {
        lighting_values.sampled_diffuse_texture = vec3(pow(texture(u_diffuse_texture, texture_coordinates).rgb, vec3(2.2f)));
    }
    else {
        lighting_values.sampled_diffuse_texture = vec3(texture(u_diffuse_texture, texture_coordinates));
    }
    lighting_values.sampled_specular_texture = vec3(1);
    lighting_values.view_dir = normalize(u_view_pos-frag_pos);

    vec3 final_color = vec3(0);

    if (u_do_point_light) {
        for (int i=0; i<u_point_lights_count; ++i) {
            final_color += calculate_blinn_phong_light(u_point_lights[i]);
        }
    }
    
    if (u_do_gamma_correction) {
        float gamma = 2.2f;
        final_color = pow(final_color, vec3(1/gamma));
    }

    frag_color = vec4(final_color, 1.0f);
}
