#version 460

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec2 texture_coordinates;

uniform mat4 u_mvp;
uniform float u_offset;

void main()
{
    float offset_x = (sin(u_offset) + 1) * 0.5f;
    float offset_y = (cos(u_offset) + 1) * 0.5f;
    float offset_z = (cos(sin(u_offset)) + 1) * 0.5f;
    gl_Position = u_mvp*vec4(aPos + vec3(offset_x, offset_y, 2*offset_z), 1.0f);
    texture_coordinates = aTexCoord;
}
