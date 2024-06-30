#shader vertex
#version 410
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec3 Normal;
out vec3 Position;

layout (std140) uniform Matrices
{
    mat4 projection;
    mat4 view;
};

uniform mat4 model;

void main()
{
    gl_Position = projection * view * model * vec4(aPos.x, aPos.y, aPos.z, 1.0);
    Normal = mat3(transpose(inverse(model))) * aNormal;
    Position = vec3(model * vec4(aPos, 1));
}

	
#shader fragment
#version 410

struct PointLight {
    vec3 position;

    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};  

in vec3 Position;
in vec3 Normal;

uniform vec3 viewPos;
uniform bool hasSkybox;
uniform samplerCube skybox;
out vec4 FragColor;
uniform float reflection;



#define NR_POINT_LIGHTS 1
uniform PointLight u_PointLights[NR_POINT_LIGHTS];


vec3 pointLightResolveDiff(PointLight light, vec3 matDiff, vec3 normal, vec3 fragPos, float attenuation);
vec3 pointLightResolveAmb(PointLight light,  vec3 matAmb, vec3 normal, vec3 fragPos, vec3 viewDir, float attenuation);
vec3 pointLightResolveSpec(PointLight light, vec3 specColor, vec3 normal, vec3 fragPos, vec3 viewDir, float attenuation);
vec3 pointLightResolve(vec3 fragPos, vec3 normal, vec3 viewDir, vec3 inputColor);

void main()
{
    vec3 I = normalize(Position - viewPos);
    vec3 R = reflect(I, normalize(Normal));

    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - Position);

    vec3 reflectColor;
    if (hasSkybox) {
        reflectColor = texture(skybox, R).rgb*reflection;
    } else {
        reflectColor = vec3(1);
    }
    vec3 lightColor = pointLightResolve(Position, norm, viewDir, reflectColor);
    FragColor = vec4(lightColor, 1.0);
}



vec3 pointLightResolveAmb(PointLight light,  vec3 matAmb, vec3 normal, vec3 fragPos, vec3 viewDir, float attenuation)
{
    return matAmb * light.ambient * attenuation;
}

vec3 pointLightResolveDiff(PointLight light, vec3 matDiff,vec3 normal, vec3 fragPos, float attenuation)
{
    vec3 lightDir = normalize(fragPos - light.position);
    float diff = max(dot(normal, -lightDir), 0.1); 
    return matDiff * light.diffuse * diff * attenuation;
}
vec3 pointLightResolveSpec(PointLight light, vec3 specColor, vec3 normal, vec3 fragPos, vec3 viewDir, float attenuation)
{
    vec3 lightDir = normalize(fragPos - light.position);
    vec3 reflectDir = reflect(lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = light.specular * spec * specColor;

    return specular * attenuation;
}

vec3 pointLightResolve(vec3 fragPos, vec3 normal, vec3 viewDir, vec3 inputColor)
{
    vec3 result = vec3(0.0, 0.0, 0.0);
    for (int i = 0; i < NR_POINT_LIGHTS; i++)
    {
        PointLight pLight = u_PointLights[i];
        float distance = length(pLight.position-fragPos);
        float attenuation = 1.0 / (pLight.constant + pLight.linear * distance + pLight.quadratic * (distance * distance));

        vec3 amb = pointLightResolveAmb(pLight, inputColor, normal, fragPos, viewDir, attenuation);
        vec3 diff = pointLightResolveDiff(pLight, inputColor, normal, fragPos, attenuation);
        vec3 specular = pointLightResolveSpec(pLight, vec3(1), normal, fragPos, viewDir, attenuation);

        result += (inputColor + specular+  amb);
    }
    return result;
}