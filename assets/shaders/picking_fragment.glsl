#version 460 core

out uvec3 fragment_color;

uniform uint u_object_id;
uniform uint u_draw_id;

void main()
{
    fragment_color = uvec3(u_object_id, u_draw_id, gl_PrimitiveID);
}
