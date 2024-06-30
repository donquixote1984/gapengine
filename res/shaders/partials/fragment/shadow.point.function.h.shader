float ShadowCalculationPointLight(PointLight pl, vec3 norm)
{
    //float epsillon = .01;
    
   // PointLight pl = u_PointLights[i];
    if (pl.castShadow == 0)
    {
        return 0;
    }
    vec3 fragToLight = FragPos-pl.position;
    vec3 lightToFrag = normalize(pl.position - FragPos);
    float epsillon = max(0.4 * (1.0 - dot(norm, lightToFrag)), 0.001);
    // float epsillon =  0.005*tan(acos(clamp(dot(norm, fragToLight), 0.0, 1.0)));
    // epsillon = clamp(epsillon, 0,0.1);
    float closestDepth = texture(u_ShadowCubes, vec4(fragToLight, pl.index)).r;
    if (closestDepth == 0)
    {
        return 1;
    }
    closestDepth *= pl.farPlane;
    float currentDepth = length(fragToLight);
    //float shadow= currentDepth - epsillon > closestDepth ? 1.0 : 0.0;
    // float bias = 0.05; // we use a much larger bias since depth is now in [near_plane, far_plane] range
    float shadow = currentDepth -  epsillon > closestDepth ? 1 : 0;        
// display closestDepth as debug (to visualize depth cubemap)
// FragColor = vec4(vec3(closestDepth / far_plane), 1.0);    
    
    //return closestDepth;
    return shadow;
}


vec3 sampleOffsetDirections[20] = vec3[]
(
   vec3( 1,  1,  1), vec3( 1, -1,  1), vec3(-1, -1,  1), vec3(-1,  1,  1), 
   vec3( 1,  1, -1), vec3( 1, -1, -1), vec3(-1, -1, -1), vec3(-1,  1, -1),
   vec3( 1,  1,  0), vec3( 1, -1,  0), vec3(-1, -1,  0), vec3(-1,  1,  0),
   vec3( 1,  0,  1), vec3(-1,  0,  1), vec3( 1,  0, -1), vec3(-1,  0, -1),
   vec3( 0,  1,  1), vec3( 0, -1,  1), vec3( 0, -1, -1), vec3( 0,  1, -1)
);
float ShadowCalculationPointLightPCF(PointLight pl, vec3 norm)
{
    float shadow = 0.0;
    int samples = 10;
    float viewDistance = length(viewPos - FragPos);
    //float diskRadius = 0.01;
    
    if (pl.castShadow == 0)
    {
        return 0;
    }
    vec3 fragToLight = FragPos - pl.position;
        vec3 lightToFrag = normalize(pl.position - FragPos);
    float epsillon = max(0.05 * (1.0 - dot(norm, lightToFrag)), 0.001);
    float diskRadius = (1.0 + (viewDistance / pl.farPlane)) / 100.0;
    float currentDepth = length(fragToLight);
    for (int j = 0; j < samples; j++)
    {
        float closestDepth = texture(u_ShadowCubes, vec4(fragToLight + sampleOffsetDirections[j] * diskRadius, pl.index)).r;
        closestDepth *= pl.farPlane;

        if (currentDepth - closestDepth > (epsillon)) {
            //return 0.5;
            shadow += 1;
        } 
    }
        //float shadow= currentDepth - epsillon > closestDepth ? 1.0 : 0.0;

    return shadow /= float(samples);
}