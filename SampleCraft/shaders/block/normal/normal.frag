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
in vec4 frag_pos_light_space;

uniform sampler2D u_texture;
uniform sampler2D u_shadow_map;
uniform vec3 u_light_pos;
uniform vec3 u_light_color;
uniform vec3 u_luna_light_pos;
uniform vec3 u_luna_light_color;

float ShadowCalculation(vec4 fragPosLightSpace)
{
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(u_shadow_map, projCoords.xy).r; 
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // check whether current frag pos is in shadow
    float shadow = currentDepth > closestDepth  ? 1.0 : 0.0;

    return shadow;
}

void main()
{
    //������룬�����ó�����ʹ���ո��ӽ�̫����
    float ambient_strength = 0.05;
    vec3 ambient = ambient_strength * vec3(1.0f, 1.0f, 1.0f);
    

    vec3 light_dir = normalize(u_light_pos - frag_pos);
    vec3 luna_light_dir = normalize(u_luna_light_pos - frag_pos);
    float diff_strength1 = max(dot(norm, light_dir), 0.0);
    float diff_strength2 = max(dot(norm, luna_light_dir), 0.0);
    vec3 diffuse1 = diff_strength1 * u_light_color * 0.045;
    vec3 diffuse2 = diff_strength2 * u_light_color * 0.045;
    float shadow = ShadowCalculation(frag_pos_light_space); 

    vec4 result = vec4(ambient + (1.0f - shadow) * (diffuse1 + diffuse2), 1.0) * texture(u_texture, tex_coord);
    FragColor = result;
}  