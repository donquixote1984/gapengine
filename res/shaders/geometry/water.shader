#shader vertex
#version 410
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

uniform mat4 model;
out vec4 FragPos;
out vec4 FragWorldPos;
out vec2 TexCoords;
out vec3 Normal;
layout (std140) uniform Matrices
{
    mat4 projection;
    mat4 view;
    mat4 clipPlane;
};

void main()
{
    FragPos = projection * view * model * vec4(aPos, 1.0);
    gl_Position = FragPos;
    FragWorldPos = model * vec4(aPos, 1.0);
    gl_ClipDistance[0] = dot(FragWorldPos, vec4(clipPlane));
    TexCoords = aTexCoords;
    Normal = aNormal;
}
	
#shader fragment
#version 410

#define MAX_POINT_LIGHTS 10
#define MAX_DIR_LIGHTS 10
uniform sampler2D u_ReflectionTexture;
uniform sampler2D u_RefractionTexture;
uniform sampler2D u_FlowTexture;

in vec4 FragPos;
in vec4 FragWorldPos;
in vec2 TexCoords;
in vec3 Normal;
out vec4 FragColor;


// pbr
uniform samplerCube u_IRMap;
uniform samplerCube u_PrefilterMap;
uniform sampler2D u_BRDFLut;


// global settings
layout (std140) uniform Settings
{
    int ub_HasHDRIMap;
    int ub_UseShadowPCF;
};


#include "../partials/fragment/material.struct.h.shader"
#include "../partials/fragment/material.function.h.shader"
#include "../partials/fragment/light.struct.h.shader"
// lights
layout (std140) uniform Lights 
{
    int ub_StaticPointLightNum;
    int ub_DynamicPointLightNum;
    int ub_StaticDirLightNum;
    int ub_DynamicDirLightNum;

    PointLight ub_StaticPointLights[MAX_POINT_LIGHTS];
    PointLight ub_DynamicPointLights[MAX_POINT_LIGHTS];
    
    DirLight ub_StaticDirLights[MAX_DIR_LIGHTS];
    DirLight ub_DynamicDirLights[MAX_DIR_LIGHTS];

    SunLight ub_SunLight;
};


uniform Water material;
uniform float time;
uniform vec3 viewPos;


vec3 waterSpecular(vec3 normal, vec3 viewDir)
{
    vec3 result = vec3(0);
    for (int i = 0; i < ub_DynamicDirLightNum; i++)
    {
        DirLight light = ub_DynamicDirLights[i];
        vec3 reflectDirection = reflect(normalize(light.direction), normal);
        float specularFactor = max(dot(reflectDirection, -viewDir), 0.0);
        specularFactor = pow(specularFactor, 16);
        result += light.diffuse * specularFactor;
    }

    return result;
}
void main()
{
    vec3 viewDir = normalize(FragWorldPos.xyz - viewPos);
    vec2 ndc = (FragPos.xy / FragPos.w) / 2.0 + 0.5;
    vec2 reflectionUV = vec2(ndc.x, 1 - ndc.y);
    vec2 refractionUV = vec2(ndc.x, ndc.y);
    vec2 uv1 = TexCoords;
    uv1.x += time * material.waveSpeed.color.r;

    vec2 distorted1 = CalcChannel(material.flow, uv1).rg * 0.1;
    distorted1 = TexCoords + vec2(distorted1.x, distorted1.y + time * material.waveSpeed.color.r);
    vec2 distorted2 = (CalcChannel(material.flow, distorted1).rg * 2.0 - 1.0) * material.waveStrength.constant;

    vec4 reflectionColor = texture(u_ReflectionTexture, clamp(reflectionUV + distorted2, 0.001, 0.999));
    vec4 refractionColor = texture(u_RefractionTexture, clamp(refractionUV + distorted2, 0.001, 0.999));

    float reflectiveFactor = dot(-viewDir, Normal);
    FragColor= mix(reflectionColor, refractionColor, reflectiveFactor);
    FragColor= mix(FragColor, vec4(material.tint.color, 1.0), 0.5);

    vec3 normal = CalcChannel(material.normal, distorted2);
    normal = vec3(normal.r *2.0 - 1,  normal.b, normal.g *2.0 - 1);  // inverse b, and g.   put normal value to vertical up
    normal = normalize(normal);


    //FragColor = vec4(dudv, 1);
    //FragColor = vec4(vec3(reflectiveFactor), 1.0);
    vec3 specular = waterSpecular(normal, viewDir);   
    FragColor = mix(reflectionColor, refractionColor, reflectiveFactor) + vec4(specular, 1);
}