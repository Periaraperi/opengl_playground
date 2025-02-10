#version 460 core

in vec3 direction;

out vec4 fragment_color;

uniform samplerCube u_cubemap;

void main()
{
    vec3 d = direction;
    //d.z = -d.z;
    fragment_color = texture(u_cubemap, d);
}
