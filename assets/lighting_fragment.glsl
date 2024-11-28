#version 460 core

in vec3 normal;
in vec3 frag_pos;
out vec4 frag_color;

uniform vec3 u_object_color;
uniform vec3 u_light_source_color;
uniform float u_ambient_strength;
uniform vec3 u_light_source_pos;
uniform vec3 u_view_pos;

void main()
{
    vec3 ambient_light = u_ambient_strength*u_light_source_color;

    vec3 light_direction = normalize(u_light_source_pos-frag_pos);
    vec3 norm = normalize(normal);
    float light_intensity = max(dot(light_direction, normal), 0.0f);
    vec3 diffuse_light = light_intensity*u_light_source_color;

    vec3 refl = reflect(-light_direction, norm);
    vec3 view_dir = normalize(u_view_pos-frag_pos);
    float specular_intensity = 0.5f;
    vec3 specular_light = specular_intensity * pow(max(dot(refl, view_dir), 0.0f), 16) * u_light_source_color;

    vec3 final_color = (ambient_light+diffuse_light+specular_light)*u_object_color;

    frag_color = vec4(final_color, 1.0f);
}


