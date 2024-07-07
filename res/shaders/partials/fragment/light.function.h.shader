
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
    vec3 lightDir = normalize(light.position - fragPos);

    vec3 targetDir;
    float spec;
    if (lightMode == LIGHT_MODE_BLINN) {
        targetDir = normalize(lightDir + viewDir);
        spec = pow(max(dot(normal, targetDir), 0.0), material.shininess.constant * 2);
    } else {
        targetDir = reflect(-lightDir, normal);
        spec = pow(max(dot(normalize(viewDir), normalize(targetDir)), 0.0), material.shininess.constant);
    }
    vec3 specular = light.specular * spec * specColor;

    return specular * attenuation;
}

vec3 pointLightResolve(vec3 fragPos, vec3 normal, vec3 viewDir, float shadow)
{
    vec3 presult = vec3(0.0, 0.0, 0.0);
    for (int i = 0; i < u_PointLightNum; i++)
    {
        PointLight pLight = ub_DynamicPointLights[i];
        float distance = length(pLight.position-fragPos);
        float attenuation = 1.0 / (pLight.constant + pLight.linear * distance + pLight.quadratic * (distance * distance));

        vec3 amb = pointLightResolveAmb(pLight, CalcChannel(material.diffuse), normal, fragPos, viewDir, attenuation);
        vec3 diff = pointLightResolveDiff(pLight, CalcChannel(material.diffuse), normal, fragPos, attenuation);
        vec3 specular = pointLightResolveSpec(pLight, CalcChannel(material.specular), normal, fragPos, viewDir, attenuation);

        presult += ((diff + specular) * (1 - shadow)+ amb);
    }
    return presult;
}

vec3 dirLightResolve(DirLight light, vec3 normal, vec3 viewDir, float shadow)
{
    for (int i = 0; i < u_DirLightNum; i++)
    {
    }
    vec3 lightDir = normalize(-light.direction);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot((viewDir), (reflectDir)), 0.0), material.shininess.constant);
    vec3 ambient = light.ambient * CalcChannel(material.diffuse);
    vec3 diffuse = light.diffuse * diff * CalcChannel(material.diffuse);
    vec3 roughness = CalcChannel(material.roughness);
    vec3 specular = light.specular * spec * CalcChannel(material.specular) * (vec3(1) - roughness);

    return ambient + (diffuse + specular) * (1 - shadow);
}
