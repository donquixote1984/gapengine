#shader vertex
#version 410


layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
{{__A_LOCATION_1__}}
{{__A_LOCATION_2__}}
{{__A_LOCATION_3__}}

layout (std140) uniform Matrices
{
    mat4 projection;
    mat4 view;
};

uniform mat4 model;


out vec3 Normal;
out vec3 FragPos;
out vec2 TexCoords;


void main()
{
    gl_Position = projection * view * model {{__POSITION_MULTIPLIER__}} * vec4(aPos.x, aPos.y, aPos.z, 1.0);
    FragPos = vec3(model * vec4(aPos, 1.0));
    Normal = mat3(transpose(inverse(model))) * aNormal;
    TexCoords = aTexCoords;
}
	

#shader fragment
#version 410

#define NR_POINT_LIGHTS 1
struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight {
    vec3 position;

    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};  

struct DirLight {
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};  

struct Channel {
    vec3 color;
    sampler2D tex;
    int hasTexture;
    bool type;
    float constant;
};
struct Material {
    Channel ambient;
    Channel diffuse;
    Channel specular;
    Channel normal;
    Channel roughness;
    Channel metalness;
    Channel shininess;
};

in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos;
out vec4 FragColor;

uniform vec3 viewPos;
uniform Material material;
uniform PointLight u_PointLights[NR_POINT_LIGHTS];
{{__U_ATTR_TILE__}}







vec3 CalcChannel(Channel c)
{
   
    vec3 color = c.color;
    vec4 tex = vec4(1.0, 1.0, 1.0, 1.0);
    if (c.hasTexture == 1)
    {
        tex = texture(c.tex, TexCoords {{__TILE_FACTOR__}});
        if (tex.a < 0.1)
        {
           // discard;
        }
    } 
    return color * vec3(tex);
}



//vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec3 matColor);
vec3 pointLightResolveDiff(PointLight light, vec3 matDiff, vec3 normal, vec3 fragPos, float attenuation);
vec3 pointLightResolveAmb(PointLight light,  vec3 matAmb, vec3 normal, vec3 fragPos, vec3 viewDir, float attenuation);
vec3 pointLightResolveSpec(PointLight light, vec3 specColor, vec3 normal, vec3 fragPos, vec3 viewDir, float attenuation);
vec3 pointLightResolve(vec3 fragPos, vec3 normal, vec3 viewDir);

void main()
{
    vec3 norm;
    if (material.normal.hasTexture == 1) {
        norm = texture(material.normal.tex, TexCoords {{__TILE_FACTOR__}}).rgb;
        norm= normalize(norm* 2.0 - 1.0);
    } else {
        norm = normalize(Normal);
    }
    vec3 viewDir = normalize(viewPos - FragPos);

    //vec3 ambient = CalcChannel(material.ambient);
    //vec3 diffuse = CalcChannel(material.diffuse);
    //vec3 specular = CalcChannel(material.specular);
    vec3 result = pointLightResolve(FragPos, norm, viewDir);
    //vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);
}

vec3 pointLightResolveAmb(PointLight light,  vec3 matAmb, vec3 normal, vec3 fragPos, vec3 viewDir, float attenuation)
{
    return matAmb * light.ambient * attenuation;
}

vec3 pointLightResolveDiff(PointLight light, vec3 matDiff,vec3 normal, vec3 fragPos, float attenuation)
{
    vec3 lightDir = normalize(fragPos - light.position);
    float diff = max(dot(normal, -lightDir), 0.0); 
    return matDiff * light.diffuse * diff * attenuation;
}
vec3 pointLightResolveSpec(PointLight light, vec3 specColor, vec3 normal, vec3 fragPos, vec3 viewDir, float attenuation)
{
    vec3 lightDir = normalize(fragPos - light.position);
    vec3 reflectDir = reflect(lightDir, normal);
    float spec = pow(max(dot(normalize(viewDir), normalize(reflectDir)), 0.0), material.shininess.constant);
    vec3 specular = light.specular * spec * specColor;

    return specular * attenuation ;
}

vec3 pointLightResolve(vec3 fragPos, vec3 normal, vec3 viewDir)
{
    vec3 result = vec3(0.0, 0.0, 0.0);
    for (int i = 0; i < NR_POINT_LIGHTS; i++)
    {
        PointLight pLight = pointLights[i];
        float distance = length(pLight.position-fragPos);
        float attenuation = 1.0 / (pLight.constant + pLight.linear * distance + pLight.quadratic * (distance * distance));

        vec3 amb = pointLightResolveAmb(pLight, CalcChannel(material.diffuse), normal, fragPos, viewDir, attenuation);
        vec3 diff = pointLightResolveDiff(pLight, CalcChannel(material.diffuse), normal, fragPos, attenuation);
        vec3 specular = pointLightResolveSpec(pLight, CalcChannel(material.specular), normal, fragPos, viewDir, attenuation);

        result += (diff + specular+ amb);
    }
    return result;
}
