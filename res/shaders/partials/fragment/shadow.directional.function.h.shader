float ShadowCalculationDirectionalLight(DirLight light, vec3 norm)
{
    int layer = GetShadowCSMLayer();
    //layer ;
    vec4 FragPosLightSpace = lightSpaceMatrices[light.index * 5 + layer] * vec4(FragPos, 1.0);
    vec3 projCoords = FragPosLightSpace.xyz / FragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;
    float currentDepth = projCoords.z;
    float bias = 0.001;
    if (projCoords.z > 1.0)
    {
        currentDepth = 1.0;
    }
    float closestDepth = texture(u_CascadeShadowMap, vec3(projCoords.xy, light.index * 5 + layer)).r;
    if (closestDepth == 0)
    {
        return 1;
    }
    float shadow = currentDepth - bias > closestDepth  ? 1.0 : 0.0;
    //float shadow = abs(currentDepth - closestDepth);
    return shadow;
}

float ShadowCalculationDirectionalLight(SunLight light, vec3 norm)
{
    int layer = GetShadowSunCSMLayer();
    vec4 FragPosLightSpace = lightSpaceMatricesSun[layer] * vec4(FragPos, 1.0);
    vec3 projCoords = FragPosLightSpace.xyz / FragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;
    float currentDepth = projCoords.z;
    float bias = 0.0001;
    if (projCoords.z > 1.0)
    {
        currentDepth = 1.0;
    }
    float closestDepth = texture(u_CascadeShadowMapSun, vec3(projCoords.xy, layer)).r;
    if (closestDepth == 0)
    {
        return 1;
    }
    float shadow = currentDepth - bias > closestDepth  ? 1.0 : 0.0;
    return shadow;
}


float ShadowCalculationDirectionalLightPCF(DirLight light, vec3 norm)
{
    int layer = GetShadowCSMLayer();
    vec4 FragPosLightSpace = lightSpaceMatrices[light.index * 5 + layer] * vec4(FragPos, 1.0);
    vec3 projCoords = FragPosLightSpace.xyz / FragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;

    float shadow = 0.0;
    //float bias = 0.0001;
    float bias = max(0.001 * (1.0 - dot(norm, normalize(-light.direction))), 0.001);
    vec3 tileSize = 1.0 / textureSize(u_CascadeShadowMap, 0);
    float currentDepth = projCoords.z;

    if (projCoords.z > 1.0)
    {
        currentDepth = 1.0;
    }

    for (int x = -2; x <=2 ; x += 1)
    {
        for (int y = -2; y <=2; y+=1)
        {
            float pcf = texture(u_CascadeShadowMap, vec3(projCoords.xy + vec2(x*2, y*2) * tileSize.xy, light.index * 5 + layer) ).r;
            shadow += (currentDepth - bias > pcf ? 1.0 : 0.0);
        }
    }

    return shadow / 25.0;
}

float ShadowCalculationDirectionalLightPCF(SunLight light, vec3 norm)
{
    int layer = GetShadowSunCSMLayer();
    vec4 FragPosLightSpace = lightSpaceMatricesSun[layer] * vec4(FragPos, 1.0);
    vec3 projCoords = FragPosLightSpace.xyz / FragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;

    float shadow = 0.0;
    //float bias = 0.0001;
    float bias = max(0.001 * (1.0 - dot(norm, normalize(-light.direction))), 0.0001);
    vec3 tileSize = 1.0 / textureSize(u_CascadeShadowMapSun, 0);
    float currentDepth = projCoords.z;

    if (projCoords.z > 1.0)
    {
        currentDepth = 1.0;
    }

    for (int x = -2; x <=2 ; x += 1)
    {
        for (int y = -2; y <=2; y+=1)
        {
            float pcf = texture(u_CascadeShadowMapSun, vec3(projCoords.xy + vec2(x*2, y*2) * tileSize.xy, layer) ).r;
            shadow += (currentDepth - bias > pcf ? 1.0 : 0.0);
        }
    }

    return shadow / 25.0;
}