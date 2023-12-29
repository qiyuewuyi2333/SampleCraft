
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec2 tex_coord;
out vec3 norm;
out vec3 frag_pos;


uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;
uniform mat4 u_tr_in_model;

void main()
{
    tex_coord = aTexCoords;
    norm = normalize( mat3(u_tr_in_model) * aNormal);
    frag_pos = vec3(u_model * vec4(aPos, 1.0));

    gl_Position = u_projection * u_view * u_model * vec4(aPos, 1.0);
}