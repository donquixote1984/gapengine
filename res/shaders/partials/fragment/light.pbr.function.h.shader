
vec3 pointLightResolvePBR(vec3 fragPos, vec3 normal, vec3 viewDir, float shadow)
{
    vec3 presult = vec3(0.0, 0.0, 0.0);
    vec3 lightColor = vec3(0.0);

    vec3 ambient;
    if(ub_HasHDRIMap == 1)    {
        vec3 ir = CalculateIR(normal, viewDir);
        vec3 specular = CalculateSpecular(normal, viewDir);
        //vec3 specular = vec3(0);
        ambient = ir + specular;
    } else {
        ambient = vec3(0.003) * CalcChannel(material.diffuse);
    }

    for (int i = 0; i < ub_DynamicPointLightNum; i++)
    {
        PointLight pLight = ub_DynamicPointLights[i];
        vec3 pbr; 
        float shadow = 0;
        if (pLight.castShadow == 1) {
            shadow = ShadowCalculationPointLight(pLight, normal);
        }
        if (shadow == 1.0)
        {
            pbr = vec3(0.0);
        } else {
            pbr = CalculatePBR(pLight, normal, viewDir);
        }

        lightColor += pbr;
    }

    //vec3 color = (lightColor + ambient) * (1 - shadow) + ambient * shadow;
    vec3 color = (lightColor + ambient);
    // HDR tonemapping
    color = color / (color + vec3(1.0));
    // gamma correct
    color = pow(color, vec3(1.0/2.2)); 
    //presult = (color * (1 - shadow)) + ambient * shadow;
    presult = color;

    return presult;
    //return vec3(shadow);
}

vec3 dirLightResolvePBR(vec3 normal, vec3 viewDir, float shadow)
{
    vec3 color = vec3(0);
    for (int i = 0; i < ub_DynamicDirLightNum; i++)
    {
        DirLight dLight = ub_DynamicDirLights[i];
        vec3 lightDir = normalize(-dLight.direction);
        float diff = max(dot(normal, lightDir), 0.0);
        vec3 reflectDir = reflect(-lightDir, normal);
        float spec = pow(max(dot((viewDir), (reflectDir)), 0.0), material.shininess.constant);
        vec3 ambient = dLight.ambient * CalcChannel(material.diffuse);
        vec3 diffuse = dLight.diffuse * diff * CalcChannel(material.diffuse);
        vec3 roughness = CalcChannel(material.roughness);
        vec3 specular = dLight.specular * spec * CalcChannel(material.specular) * (vec3(1) - roughness);
        color += ambient + (diffuse + specular) * (1 - shadow);
    }
    return color;
}

vec3 PBRLighting(vec3 normal, vec3 viewDir)
{
    vec3 presult    = vec3(0.0, 0.0, 0.0);
    vec3 dresult    = vec3(0.0, 0.0, 0.0);
    vec3 sunresult  = vec3(0.0, 0.0, 0.0);
    vec3 ambient    = vec3(0.0, 0.0, 0.0);
    vec3 skyambient = vec3(0.0, 0.0, 0.0);
    for (int i = 0; i < ub_DynamicPointLightNum; i++)
    {
        PointLight pLight = ub_DynamicPointLights[i];
        vec3 color; 
        float shadow = 0;
        if (pLight.castShadow == 1) {
            shadow = ShadowCalculationPointLight(pLight, normal);
        }
        if (shadow == 1.0)
        {
            color = vec3(0.0);
        } else {
            color = CalculatePBR(pLight, normal, viewDir);
        }

        presult += color;
    }

    for (int i = 0; i < ub_DynamicDirLightNum; i++)
    {
        DirLight dLight = ub_DynamicDirLights[i];
        vec3 color; 
        float shadow = 0.0;
        if (dLight.castShadow == 1)
        {
            if (ub_UseShadowPCF == 0) 
            {
                shadow = ShadowCalculationDirectionalLight(dLight, normal);
            } else {
                shadow = ShadowCalculationDirectionalLightPCF(dLight, normal);
            }
        }

        color = shadow * vec3(0.0) + (1 - shadow) * CalculatePBR(dLight, normal, viewDir);
        //color =  CalculatePBR(dLight, normal, viewDir);
        //color = vec3(shadow);

        dresult += color;
    }
    
    if (ub_SunLight.enable == 1)
    {
        float shadow = 0.0;
        if (ub_SunLight.castShadow == 1) {
            if (ub_UseShadowPCF == 0) 
            {
                shadow = ShadowCalculationDirectionalLight(ub_SunLight, normal);
            } else {
                shadow = ShadowCalculationDirectionalLightPCF(ub_SunLight, normal);
            }
        }
        
        vec3 sun_ir = CalculatePBR(ub_SunLight, normal, viewDir, ScreenSpaceDir, skyambient);

        //skyambient = (skyambient)  * CalcChannel(material.diffuse);
        skyambient = CalculateIRSky(normal, viewDir, skyambient + sun_ir);
        sunresult = shadow * vec3(0) + (1 - shadow) * sun_ir;
 
    }
   

     if(ub_HasHDRIMap == 1)    {
        vec3 ir = CalculateIR(normal, viewDir);
        vec3 specular = CalculateSpecular(normal, viewDir);
        //vec3 specular = vec3(0);
        ambient = ir + specular;
    } else {
        ambient = vec3(0.03) * CalcChannel(material.diffuse);
    }

    vec3 color = presult + dresult + sunresult + ambient + skyambient;
    //vec3 color = sunresult;

    color = color / (color + vec3(1.0));
    // gamma correct
    color = pow(color, vec3(1.0/2.2)); 

    //    int layer = GetShadowCSMLayer();
        /*
    if (layer == 0) 
    {
        color *= vec3(1,0,0);
    }
    if (layer == 1) 
    {
        color *= vec3(0,1,0);
    }
    if (layer == 2) 
    {
        color *= vec3(0,0,1);
    }
    if (layer == 3) 
    {
        color *= vec3(1,1,0);
    }
     if (layer == 4) 
    {
        color *= vec3(0,1,1);
    } */

    /*
       vec3 in_scatter = GetSkyRadianceToPoint(camera - earth_center,
        point - earth_center, shadow_length, sun_direction, transmittance); 
    */
    if (ub_SunLight.enable == 1)
    {
        vec3 transmittance = vec3(0);
        //vec3 sunlight = GetSkyRadianceToPoint(ub_SunLight.depth, FragPos - earth_center, 0, -ub_SunLight.direction, transmittance); 
        {{__ATOMSPHERE_TRANSMITTANCE__}}
        color = mix(color, vec3(1, 1, 1.1), 1-transmittance.r);
    }
    return color;
}
