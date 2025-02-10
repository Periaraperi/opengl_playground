#version 460 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 18) out;

out vec4 frag_pos;

uniform mat4 u_light_projection;
uniform mat4 u_lights_views[6];

void main()
{
    for (int face=0; face<6; ++face) {
        gl_Layer = face;
        // loop for each triangle vertex
        for (int i=0; i<3; ++i) {
            frag_pos = gl_in[i].gl_Position;
            gl_Position = u_light_projection*u_lights_views[face]*frag_pos;
            EmitVertex();
        }
        EndPrimitive();
    }
}
