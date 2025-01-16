#version 460 core

layout (triangles) in;
layout (line_strip, max_vertices = 6) out;

uniform mat4 u_projection;

in VS_OUT {
    vec3 norm;
} geometry_in[];

void main()
{
    for (int i=0; i<3; ++i) {
        gl_Position = u_projection*gl_in[i].gl_Position;
        EmitVertex();

        gl_Position = u_projection*(gl_in[i].gl_Position + vec4(geometry_in[i].norm, 0.0f)*0.1f);
        EmitVertex();
        EndPrimitive();
    }
}
