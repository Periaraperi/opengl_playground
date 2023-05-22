#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec4 aColor;
layout (location = 2) in vec2 aTexCoord;
  
out vec4 vertex_color;
out vec2 tex_coord;
uniform mat4 u_mvp;

void main()
{
    gl_Position = u_mvp*vec4(aPos,0.0f,1.0f); 
    vertex_color = aColor;
    tex_coord = aTexCoord;
}

