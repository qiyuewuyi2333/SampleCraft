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

uniform sampler2D u_texture;
uniform vec3 u_light_pos;
uniform vec3 u_light_color;
uniform vec3 u_luna_light_pos;
uniform vec3 u_luna_light_color;
//the specific color of texture, such as the green of leave
uniform vec4 u_texture_color;

void main()
{
    //增大距离，并设置常量，使光照更接近太阳光
    float ambient_strength = 0.05;
    vec3 ambient = ambient_strength * vec3(1.0f, 1.0f, 1.0f);
    

    vec3 light_dir = normalize(u_light_pos - frag_pos);
    vec3 luna_light_dir = normalize(u_luna_light_pos - frag_pos);
    float diff_strength1 = max(dot(norm, light_dir), 0.0);
    float diff_strength2 = max(dot(norm, luna_light_dir), 0.0);
    vec3 diffuse1 = diff_strength1 * u_light_color * 0.045;
    vec3 diffuse2 = diff_strength2 * u_light_color * 0.045;
    vec4 texture1 = texture(u_texture, tex_coord);
    if(texture1.a<0.5)  
        discard;
    vec4 result = vec4((ambient + diffuse1 + diffuse2), 1.0) * texture1 * u_texture_color;

    FragColor = result;
}  