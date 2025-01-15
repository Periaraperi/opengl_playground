#version 460 core

layout (points) in;
layout (points, max_vertices = 1) out;
//layout (line_strip, max_vertices = 2) out;

void main()
{
    gl_PointSize = 20.0f;
    gl_Position = gl_in[0].gl_Position;
    EmitVertex();

    //gl_Position = gl_in[0].gl_Position + vec4(-0.1f, 0.0f, 0.0f, 0.0f);
    //EmitVertex();

    //gl_Position = gl_in[0].gl_Position + vec4(0.1f, 0.0f, 0.0f, 0.0f);
    //EmitVertex();

    EndPrimitive();
}
