#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

// NOTE: lighting calculations are done in World Space

out vec3 color;

uniform mat4 u_vp;
uniform mat4 u_model;

uniform float u_ambient_strength;
uniform float u_specular_intensity;
uniform int u_specular_coefficient;

uniform vec3 u_light_source_color;

uniform vec3 u_light_source_pos;
uniform vec3 u_view_pos;

void main()
{
    gl_Position = u_vp*u_model*vec4(aPos, 1.0f);

    vec3 normal = mat3(inverse(transpose(u_model)))*aNormal; // do this on cpu and pass as uniform.
    vec3 frag_pos = vec3(u_model*vec4(aPos, 1.0f));

    vec3 ambient_light = u_ambient_strength*u_light_source_color;

    vec3 light_direction = normalize(u_light_source_pos-frag_pos); // from pixel to light_source
    vec3 norm = normalize(normal); // normal from surface
    float light_intensity = max(dot(light_direction, normal), 0.0f);
    vec3 diffuse_light = light_intensity*u_light_source_color;

    // negate light_direction to get vector from light source to object surface,
    // then reflect it on surface normal to get reflection from surface to camera
    vec3 refl = reflect(-light_direction, norm);
    vec3 view_dir = normalize(u_view_pos-frag_pos);
    vec3 specular_light = u_specular_intensity * pow(max(dot(refl, view_dir), 0.0f), u_specular_coefficient) * u_light_source_color;

    color = (ambient_light+diffuse_light+specular_light);
}

