#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

out vec3 normal;
out vec3 frag_pos; // fragments position in world space

uniform mat4 u_vp;
uniform mat4 u_model;

void main()
{
    gl_Position = u_vp*u_model*vec4(aPos, 1.0f);
    //normal = aNormal;
    normal = mat3(inverse(transpose(u_model)))*aNormal; // do this on cpu and pass as uniform.
    frag_pos = vec3(u_model*vec4(aPos, 1.0f));
}

