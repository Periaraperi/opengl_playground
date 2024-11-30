#version 460 core

in vec3 normal;
in vec3 frag_pos;
in vec3 light_source_pos; // get this from vertex shader converted to view space
out vec4 frag_color;

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess; // specular coefficient
};

struct Light {
    vec3 pos; // light source position, we don't use this here. We supply world pos in VS that gets converted to view space

    // intensity of all 3 lights in all channels
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Material u_material;
uniform Light u_light;

void main()
{
    vec3 ambient_light = u_light.ambient * u_material.ambient;

    vec3 light_direction = normalize(light_source_pos-frag_pos); // from pixel to light_source
    vec3 norm = normalize(normal); // normal from surface
    float light_intensity = max(dot(light_direction, normal), 0.0f);
    vec3 diffuse_light = u_light.diffuse * light_intensity * u_material.diffuse;

    // negate light_direction to get vector from light source to object surface,
    // then reflect it on surface normal to get reflection from surface to camera
    vec3 refl = reflect(-light_direction, norm);
    vec3 view_dir = normalize(-frag_pos); // since camera pos in view space is on origin, view_dir = (0,0,0) - frag_pos
    float specular_intensity = pow(max(dot(refl, view_dir), 0.0f), u_material.shininess);
    vec3 specular_light = u_light.specular * specular_intensity * u_material.specular;

    vec3 final_color = (ambient_light + diffuse_light + specular_light);
    frag_color = vec4(final_color, 1.0f);
}
