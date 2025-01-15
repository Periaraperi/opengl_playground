#version 460 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in VS_OUT {
    vec3 normal;
    vec2 tex_coords;
} geometry_in[];

out vec2 texture_coordinates;

uniform float u_fake_time;

void main()
{
    vec3 a = normalize(gl_in[0].gl_Position - gl_in[1].gl_Position).xyz;
    vec3 b = normalize(gl_in[2].gl_Position - gl_in[1].gl_Position).xyz;
    vec3 N = cross(b, a);
    for (int i=0; i<3; ++i) {
        gl_Position = gl_in[i].gl_Position + vec4(u_fake_time*N, 1.0f);
        texture_coordinates = geometry_in[i].tex_coords;
        EmitVertex();
    }

    EndPrimitive();
}
