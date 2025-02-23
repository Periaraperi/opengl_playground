#version 460

layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in vec4 aColor;
layout (location = 3) in float aTexSlot;

out VS_DATA {
    vec2 texture_coordinates;
    vec4 color;
    float texture_slot;
} vs_data;

uniform mat4 u_mvp;

void main()
{
    gl_Position = u_mvp*vec4(aPos, 0.0f, 1.0f);
    vs_data.texture_coordinates = aTexCoord;
    vs_data.color = aColor;
    vs_data.texture_slot = aTexSlot;
}
