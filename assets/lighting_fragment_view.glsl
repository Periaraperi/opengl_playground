#version 460 core

in vec3 normal;
in vec3 frag_pos;
in vec3 light_source_pos; // get this from vertex shader converted to view space
out vec4 frag_color;

uniform float u_ambient_strength;
uniform float u_specular_intensity;
uniform int u_specular_coefficient;

uniform vec3 u_object_color;
uniform vec3 u_light_source_color;

void main()
{
    vec3 ambient_light = u_ambient_strength*u_light_source_color;

    vec3 light_direction = normalize(light_source_pos-frag_pos); // from pixel to light_source
    vec3 norm = normalize(normal); // normal from surface
    float light_intensity = max(dot(light_direction, normal), 0.0f);
    vec3 diffuse_light = light_intensity*u_light_source_color;

    // negate light_direction to get vector from light source to object surface,
    // then reflect it on surface normal to get reflection from surface to camera
    vec3 refl = reflect(-light_direction, norm);
    vec3 view_dir = normalize(-frag_pos); // since camera pos in view space is on origin, view_dir = (0,0,0) - frag_pos
    vec3 specular_light = u_specular_intensity * pow(max(dot(refl, view_dir), 0.0f), u_specular_coefficient) * u_light_source_color;

    vec3 final_color = (ambient_light+diffuse_light+specular_light)*u_object_color;

    frag_color = vec4(final_color, 1.0f);
}
