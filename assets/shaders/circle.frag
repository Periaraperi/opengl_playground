#version 460 core

in vec2 center;
in float radius;
in vec4 color;

out vec4 frag_color;

void main()
{
    vec2 p = gl_FragCoord.xy;
    p -= center; // translate center to (0,0)

    float dis = radius-length(p);
    float st = smoothstep(0.0f,2.0f,dis); // play with second param here
    //float st = step(0.0f,dis);
    frag_color = vec4(color.rgb,st*color.a);
}
