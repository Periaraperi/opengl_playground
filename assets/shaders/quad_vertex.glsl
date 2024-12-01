#version 460 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexCoordinates;
layout (location = 2) in vec4 aColor;
layout (location = 3) in float aTextureUnit;

out vec2 tex_coordinates;
out vec4 color;
out float texture_unit;
uniform mat4 u_mvp;

void main()
{
    gl_Position = u_mvp*vec4(aPos, 0.0f, 1.0f); 
    tex_coordinates = aTexCoordinates;
    color = aColor;
    texture_unit = aTextureUnit;
}
