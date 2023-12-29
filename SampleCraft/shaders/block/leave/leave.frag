#version 330 core

out vec4 FragColor;

struct Material
{
    sampler2D diffuse;
    sampler2D specualar;
    float     shininess;
};

in vec2 tex_coord;
in vec3 norm;
in vec3 frag_pos;

uniform sampler2D u_test_texture;
uniform vec3 u_leave_color;
uniform vec3 u_light_color;
uniform vec3 u_light_pos;


void main()
{
    
    //增大距离，并设置常量，使光照更接近太阳光
    float ambient_strength = 0.005;
    vec3 ambient = ambient_strength * u_light_color;

    vec3 light_dir = normalize(u_light_pos - frag_pos);
    float diff_strength = max(dot(norm, light_dir), 0.0);
    vec3 diffuse = diff_strength * u_light_color * 0.045;
    vec4 texture_color = texture(u_test_texture, tex_coord);
    vec4 result = vec4((ambient + diffuse), 1.0)* texture_color;
    if(texture_color.a<0.4f)
        discard;
    FragColor = result * vec4(u_leave_color, 1.0);
}  