#version 330 core

in vec4 vertex_color;
in vec2 tex_coord;
in float tex_id;

out vec4 frag_color;

uniform sampler2D u_textures[3];

void main()
{
    int id = int(tex_id);
    switch (id) {
        case 0: frag_color = texture(u_textures[0],tex_coord)*vertex_color; break;
        case 1: frag_color = texture(u_textures[1],tex_coord)*vertex_color; break;
        case 2: frag_color = texture(u_textures[2],tex_coord)*vertex_color; break;
    }
}
