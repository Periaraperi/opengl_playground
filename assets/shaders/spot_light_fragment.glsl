#version 460 core

// NOTE: lighting calculations are done in World Space

in vec3 normal;
in vec3 frag_pos;
in vec2 texture_coordinates;
out vec4 frag_color;

struct Material {
    sampler2D diffuse_texture;
    sampler2D specular_texture;

    float shininess; // specular coefficient
};

struct Light {
    vec3 pos; // pos for spot light in world
    vec3 direction;
    float angle; // Note: cosine of angle that forms cone radius
    float outer_angle;
    float angle_delta;

    // intensity of all 3 lights in all channels
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    // attenuation equation coefficients
    float constant;
    float linear;
    float quadratic;
};

uniform Material u_material;
uniform Light u_light;

uniform vec3 u_view_pos;

void main()
{
    vec3 light_direction = normalize(u_light.pos-frag_pos); // from pixel to light_source
    //float a = dot(light_direction, normalize(-u_light.direction));
    //float b = u_light.angle - u_light.outer_angle;
    //float cone_smoothing = clamp((a - u_light.outer_angle) / b, 0.0f, 1.0f);
    float phi = dot(light_direction, normalize(-u_light.direction));
    //float cone_smoothing = 1.0f - ((1.0f - phi)/(1.0f - u_light.angle));
    float cone_smoothing = 0;
    if (phi > u_light.angle) {
        cone_smoothing = 1.0f;
    }
    else if (phi < u_light.outer_angle) {
        cone_smoothing = 0.0f;
    }
    else {
        cone_smoothing = (phi-u_light.outer_angle) / u_light.angle_delta;
    }

    float d = length(u_light.pos-frag_pos);
    float attenuation = 1.0f / (u_light.constant + u_light.linear*d + u_light.quadratic*d*d);

    vec3 ambient_light = u_light.ambient * vec3(texture(u_material.diffuse_texture, texture_coordinates)) * attenuation;

    vec3 norm = normalize(normal); // normal from surface
    float light_intensity = max(dot(light_direction, normal), 0.0f);
    vec3 diffuse_light = u_light.diffuse * light_intensity * vec3(texture(u_material.diffuse_texture, texture_coordinates)) * attenuation * cone_smoothing;

    // negate light_direction to get vector from light source to object surface,
    // then reflect it on surface normal to get reflection from surface to camera
    vec3 refl = reflect(-light_direction, norm);
    vec3 view_dir = normalize(u_view_pos-frag_pos);
    float specular_intensity = pow(max(dot(refl, view_dir), 0.0f), u_material.shininess);
    vec3 specular_light = u_light.specular * specular_intensity * vec3(texture(u_material.specular_texture, texture_coordinates)) * attenuation * cone_smoothing;

    vec3 final_color = (ambient_light + diffuse_light + specular_light);
    frag_color = vec4(final_color, 1.0f);
}
