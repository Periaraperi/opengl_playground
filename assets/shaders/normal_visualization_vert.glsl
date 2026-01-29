#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

out VS_DATA {
    vec3 normal;
} vs_out;

uniform mat4 u_view;
uniform mat4 u_model;

void main()
{
    mat3 normal_matrix = mat3(transpose(inverse(u_view * u_model)));
    vs_out.normal = vec3(vec4(normal_matrix * aNormal, 0.0f));
    gl_Position = u_view * u_model * vec4(aPos, 1.0f); 
}
