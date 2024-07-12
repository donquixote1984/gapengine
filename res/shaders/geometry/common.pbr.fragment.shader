#shader fragment
#version 410


{{{__SHADOW_FRAG_HEADER__}}}

{{> fragment/define.h.shader}}

{{> fragment/material.struct.h.shader}}

{{> fragment/light.struct.h.shader}}

{{> fragment/io.h.shader}}

{{> fragment/uniforms.h.shader}}

{{> fragment/csm.h.shader}}

{{> fragment/settings.h.shader}}

{{> fragment/material.function.h.shader}}

{{> fragment/pbr.function.h.shader}}

{{> fragment/shadow.point.function.h.shader}}

{{> fragment/shadow.directional.function.h.shader}}

{{> fragment/shadow.function.h.shader}}

{{> fragment/light.pbr.function.h.shader}}




void main()
{
    vec3 norm;

    {{> fragment/normal.shader}}

    vec3 viewDir = normalize(viewPos - FragPos);
    //vec3 ambient = CalcChannel(material.ambient);
    //vec3 diffuse = CalcChannel(material.diffuse);
    //vec3 specular = CalcChannel(material.specular);
    {{{__SHADOW_FRAG_MAIN__}}}

    
    //vec3 d_result = dirLightResolvePBR(norm, viewDir, shadow);
//    vec3 result = pointLightResolve(FragPos, norm, viewDir, shadow);
    //vec3 p_result = pointLightResolvePBR(FragPos, norm, viewDir, shadow);
    //vec3 result = ambient + diffuse + specular;
    vec3 result = PBRLighting(norm, viewDir);
    
    FragColor = vec4(result, 1);
    //FragColor = vec4(CalcChannel(material.diffuse), 1);
    //FragColor = vec4(vec3(1), 1);
}