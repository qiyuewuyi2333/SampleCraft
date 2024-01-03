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
    // calculate bias (based on depth map resolution and slope)
    vec3 normal = normalize(norm);
    vec3 lightDir = normalize(u_light_pos - frag_pos);
    float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);
    // check whether current frag pos is in shadow
    // float shadow = currentDepth - bias > closestDepth  ? 1.0 : 0.0;
    // PCF
    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(u_shadow_map, 0);
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(u_shadow_map, projCoords.xy + vec2(x, y) * texelSize).r; 
            shadow += currentDepth - bias > pcfDepth  ? 1.0 : 0.0;        
        }    
    }
    shadow /= 9.0;
    
    // keep the shadow at 0.0 when outside the far_plane region of the light's frustum.
    if(projCoords.z > 1.0)
        shadow = 0.0;
        
    return shadow;
}

void main()
{
    //增大距离，并设置常量，使光照更接近太阳光
    float ambient_strength = 0.05;
    vec3 ambient = ambient_strength * vec3(1.0f, 1.0f, 1.0f);
    

    vec3 light_dir = normalize(u_light_pos - vec3(0.0f, 0.0f, 0.0f));


    float diff_strength1 = max(dot(norm, light_dir), 0.0);
    vec3 diffuse1 = diff_strength1 * u_light_color;
    float bias = max(0.05 * (1.0 - dot(norm, light_dir)), 0.005);  

    float shadow = ShadowCalculation(frag_pos_light_space); 
    
    vec4 result = vec4(ambient + (1.0f - shadow) * (diffuse1 ), 1.0) * texture(u_texture, tex_coord);
    FragColor = result;
}  