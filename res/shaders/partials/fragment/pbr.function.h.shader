vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}
vec3 fresnelSchlickRoughness(float cosTheta, vec3 F0, float roughness)
{
    return F0 + (max(vec3(1.0 - roughness), F0) - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}

float DistributionGGX(vec3 normal, vec3 halfVector, float roughness)
{
    float a = roughness * roughness;
    float a2 = a * a;
    float normalDotHalfVector = max(dot(normal, halfVector), 0.0);
    float normalDotHalfVector2 = normalDotHalfVector * normalDotHalfVector;

    float num = a2;
    float denom = (normalDotHalfVector2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;
    return num / denom;
}

float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = roughness + 1.0;
    float k = (r * r) / 8.0;
    float num = NdotV;
    float denom = NdotV * (1.0 - k) + k;
    return num / denom;
}

float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2  = GeometrySchlickGGX(NdotV, roughness);
    float ggx1  = GeometrySchlickGGX(NdotL, roughness);
    return ggx1 * ggx2;
}
// viewDir
// FragPos
// Normal
/*

struct Material {
    Channel ambient;
    Channel diffuse;
    Channel specular;
    Channel normal;
    Channel roughness;
    Channel metalness;
    Channel shininess;
};
*/
/*

struct PointLight {
    vec3 position;

    float constant;
    float linear;
    float quadratic;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    int castShadow;
    float farPlane;
};  

*/
vec3 CalculatePBR(PointLight light, vec3 norm, vec3 viewDir)
{
    vec3 L = normalize(light.position - FragPos);
    vec3 H = normalize(viewDir + L);
    
    float distance    = length(light.position - FragPos);
    float attenuation = 1.0 / (distance * distance);
    vec3  radiance    = light.diffuse * attenuation;   

    vec3 F0 = vec3(0.04); 
    F0 = mix(F0, CalcChannel(material.diffuse), CalcChannel(material.metalness).r);

    float NDF = DistributionGGX(norm, H, CalcChannel(material.roughness).r );        
    float G   = GeometrySmith(norm, viewDir, L, CalcChannel(material.roughness).r );      
    vec3 F    = fresnelSchlick(max(dot(H, viewDir), 0.0), F0);   

    vec3 kS = F;
    vec3 kD = vec3(1.0) - kS;
    kD *= 1.0 - CalcChannel(material.metalness);     
    vec3 nominator    = NDF * G * F;
    float denominator = 4.0 * max(dot(norm, viewDir), 0.0) * max(dot(norm, L), 0.0) + 0.001; 
    vec3 specular     = nominator / denominator;

    float NdotL = max(dot(norm, L), 0.0);        
    return (kD * CalcChannel(material.diffuse)/ PI + specular) * radiance * NdotL;
}


vec3 __CalculatePBR_Directional__(vec3 lightDir, vec3 diffuse, vec3 norm, vec3 viewDir)
{
    vec3 L = lightDir;
    vec3 H = normalize(viewDir + L);
    
    //float distance    = length(light.position - FragPos);
    //float attenuation = 1.0 / (distance * distance);
    vec3  radiance = diffuse;   

    vec3 F0 = vec3(0.04); 
    F0 = mix(F0, CalcChannel(material.diffuse), CalcChannel(material.metalness).r);

    float NDF = DistributionGGX(norm, H, CalcChannel(material.roughness).r );        
    float G   = GeometrySmith(norm, viewDir, L, CalcChannel(material.roughness).r );      
    vec3 F    = fresnelSchlick(max(dot(H, viewDir), 0.0), F0);   

    vec3 kS = F;
    vec3 kD = vec3(1.0) - kS;
    kD *= 1.0 - CalcChannel(material.metalness);     
    vec3 nominator    = NDF * G * F;
    float denominator = 4.0 * max(dot(norm, viewDir), 0.0) * max(dot(norm, L), 0.0) + 0.001; 
    vec3 specular     = nominator / denominator;

    float NdotL = max(dot(norm, L), 0.0);        
    return (kD * CalcChannel(material.diffuse)/ PI + specular) * radiance * NdotL;
}

vec3 CalculatePBR(DirLight light, vec3 norm, vec3 viewDir)
{
    vec3 L = -normalize(light.direction);
    return __CalculatePBR_Directional__(L, light.diffuse, norm, viewDir);;
    //return light.diffuse;
}

vec3 CalculatePBR(SunLight light, vec3 norm, vec3 viewDir, vec3 screenViewRay, out vec3 sky_irradiance)
{
    vec3 L = -normalize(light.direction);
    vec3 sun_irradiance = vec3(0);
    //__SUN_IRRADIANCE__ is : sun_irradiance = GetSunAndSkyIrradiance(light.depth, norm, L, sky_irradiance);
    {{__SUN_IRRADIANCE__}}
    return __CalculatePBR_Directional__(L, sun_irradiance, norm, viewDir);
}


vec3 CalculateIR(vec3 norm, vec3 viewDir)
{
    vec3 F0 = vec3(0.04); 
    F0 = mix(F0, CalcChannel(material.diffuse), CalcChannel(material.metalness).r);
    vec3 kS = fresnelSchlickRoughness(max(dot(norm, viewDir), 0.0), F0, CalcChannel(material.roughness).r);
    vec3 kD = 1.0 - kS;
    kD *= 1.0 - CalcChannel(material.metalness);	  
    vec3 ir = texture(u_IRMap, norm).rgb;
    vec3 diffuse = ir * CalcChannel(material.diffuse);
    return (kD * diffuse);
}

vec3 CalculateIRSky(vec3 norm, vec3 viewDir, vec3 skyIrr)
{
    vec3 F0 = vec3(0.04); 
    F0 = mix(F0, CalcChannel(material.diffuse), CalcChannel(material.metalness).r);
    vec3 kS = fresnelSchlickRoughness(max(dot(norm, viewDir), 0.0), F0, CalcChannel(material.roughness).r);
    vec3 kD = 1.0 - kS;
    kD *= 1.0 - CalcChannel(material.metalness);	  
    vec3 ir = skyIrr;
    vec3 diffuse = ir * CalcChannel(material.diffuse);
    return (kD * diffuse);
}


vec3 CalculateSpecular(vec3 norm, vec3 viewDir)
{
    vec3 R = reflect(-viewDir, norm);
    vec3 F0 = vec3(0.04); 
    F0 = mix(F0, CalcChannel(material.diffuse), CalcChannel(material.metalness).r);
    vec3 F = fresnelSchlickRoughness(max(dot(norm, viewDir), 0.0), F0, CalcChannel(material.roughness).r);
    vec3 prefilteredColor = textureLod(u_PrefilterMap, R,   CalcChannel(material.roughness).r* MAX_REFLECTION_LOD).rgb;    
    vec2 brdf  = texture(u_BRDFLut, vec2(max(dot(norm, viewDir), 0.0), CalcChannel(material.roughness).r)).rg;
    vec3 spec = prefilteredColor * (F * brdf.x + brdf.y);
    return spec;
}