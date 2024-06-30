
uniform vec3 viewPos;
uniform Material material;
//uniform PointLight u_PointLights[MAX_POINT_LIGHTS];

//uniform int u_PointLightNum;
uniform int u_CombineNormal;

// pbr
uniform samplerCube u_IRMap;
uniform samplerCube u_PrefilterMap;
uniform sampler2D u_BRDFLut;

// shadow

uniform sampler2DArray u_CascadeShadowMap;
uniform sampler2DArray u_CascadeShadowMapSun;
uniform samplerCubeArray u_ShadowCubes;



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

// global settings
layout (std140) uniform Settings
{
    int ub_HasHDRIMap;
    int ub_UseShadowPCF;
};
{{__U_ATTR_TILE__}}