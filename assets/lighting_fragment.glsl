#version 460 core

// NOTE: lighting calculations are done in World Space

in vec3 normal;
in vec3 frag_pos;
in vec2 texture_coordinates;
out vec4 frag_color;

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess; // specular coefficient
};

struct Light {
    vec3 pos; // light source position

    // intensity of all 3 lights in all channels
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Material u_material;
uniform Light u_light;

uniform vec3 u_view_pos;

uniform sampler2D u_texture;

void main()
{
    vec3 ambient_light = u_light.ambient * u_material.ambient;

    vec3 light_direction = normalize(u_light.pos-frag_pos); // from pixel to light_source
    vec3 norm = normalize(normal); // normal from surface
    float light_intensity = max(dot(light_direction, normal), 0.0f);
    vec3 diffuse_light = u_light.diffuse * light_intensity * u_material.diffuse;

    // negate light_direction to get vector from light source to object surface,
    // then reflect it on surface normal to get reflection from surface to camera
    vec3 refl = reflect(-light_direction, norm);
    vec3 view_dir = normalize(u_view_pos-frag_pos);
    float specular_intensity = pow(max(dot(refl, view_dir), 0.0f), u_material.shininess);
    vec3 specular_light = u_light.specular * specular_intensity * u_material.specular;

    vec3 final_color = (ambient_light + diffuse_light + specular_light);
    frag_color = texture(u_texture, texture_coordinates)*vec4(final_color, 1.0f);
}
