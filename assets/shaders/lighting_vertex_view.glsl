#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec3 normal; // normals position in view space
out vec3 frag_pos; // fragments position in view space
out vec3 light_source_pos; // light source position in view space
out vec2 texture_coordinates;

uniform mat4 u_projection;
uniform mat4 u_view;
uniform mat4 u_model;

uniform vec3 u_light_source_pos; // in world space

void main()
{
    mat4 view_model = u_view*u_model;
    gl_Position = u_projection*view_model*vec4(aPos, 1.0f);
    normal = mat3(inverse(transpose(view_model)))*aNormal;
    frag_pos = vec3(view_model*vec4(aPos, 1.0f));
    light_source_pos = vec3(u_view*vec4(u_light_source_pos, 1.0f));
    texture_coordinates = aTexCoords;
}
