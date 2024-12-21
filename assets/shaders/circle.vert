#version 460 core
layout (location = 0) in vec2 aPos; // normalised quad pos
layout (location = 1) in vec2 aCenter; // circle center in world space
layout (location = 2) in float aRadius;
layout (location = 3) in vec4 aColor;
  
out vec2 center;
out float radius;
out vec4 color;
uniform mat4 u_mvp;

void main()
{
    gl_Position = u_mvp*vec4(aPos,0.0f,1.0f); 
    center = aCenter;
    radius = aRadius;
    color = aColor;
}
