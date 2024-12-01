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
    vec3 pos; // pos for point light

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
    float d = length(u_light.pos-frag_pos);
    float attenuation = 1.0f / (u_light.constant + u_light.linear*d + u_light.quadratic*d*d);

    vec3 ambient_light = u_light.ambient * vec3(texture(u_material.diffuse_texture, texture_coordinates)) * attenuation;

    vec3 light_direction = normalize(u_light.pos-frag_pos); // from pixel to light_source
    vec3 norm = normalize(normal); // normal from surface
    float light_intensity = max(dot(light_direction, normal), 0.0f);
    vec3 diffuse_light = u_light.diffuse * light_intensity * vec3(texture(u_material.diffuse_texture, texture_coordinates)) * attenuation;

    // negate light_direction to get vector from light source to object surface,
    // then reflect it on surface normal to get reflection from surface to camera
    vec3 refl = reflect(-light_direction, norm);
    vec3 view_dir = normalize(u_view_pos-frag_pos);
    float specular_intensity = pow(max(dot(refl, view_dir), 0.0f), u_material.shininess);
    vec3 specular_light = u_light.specular * specular_intensity * vec3(texture(u_material.specular_texture, texture_coordinates)) * attenuation;

    vec3 final_color = (ambient_light + diffuse_light + specular_light);
    frag_color = vec4(final_color, 1.0f);
}
