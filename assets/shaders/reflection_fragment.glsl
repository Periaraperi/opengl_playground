#version 460 core

in vec3 position;
in vec3 normal;

out vec4 fragment_color;

uniform samplerCube u_cubemap;
uniform vec3 u_camera_pos;

void main()
{
    vec3 I = normalize(position-u_camera_pos);
    vec3 R = reflect(I, normalize(normal));
    fragment_color = vec4(texture(u_cubemap, R).rgb, 1.0f);
}
