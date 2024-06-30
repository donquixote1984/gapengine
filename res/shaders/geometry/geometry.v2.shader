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
{{> vertex/uniforms.h.shader}}
{{__SHADOW_VERT_HEADER__}}

out vec3 Normal;
out vec3 FragPos;
out vec3 FragViewPos;
out vec2 TexCoords;
{{__A_OUT_1__}}
{{__A_OUT_2__}}
{{__A_OUT_3__}}

void main()
{
    gl_Position = projection * view * model {{__POSITION_MULTIPLIER__}} * vec4(aPos.x, aPos.y, aPos.z, 1.0);
    FragPos = vec3(model {{__POSITION_MULTIPLIER__}}* vec4(aPos, 1.0));
    FragViewPos = vec3(view * model {{__POSITION_MULTIPLIER__}} * vec4(aPos, 1.0));
    mat3 normalMatrix = mat3(transpose(inverse(model {{__POSITION_MULTIPLIER__}})));

    Normal = normalize(normalMatrix * aNormal);
    TexCoords = aTexCoords;
    {{{__TBN__}}}
    {{{__SHADOW_VERT_MAIN__}}}
    {{{__INSTANCING_U__}}}
}
	

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

{{> fragment/light.function.h.shader}}

{{> fragment/shadow.point.function.h.shader}}

{{> fragment/shadow.directional.function.h.shader}}

{{> fragment/shadow.function.h.shader}}

void main()
{
    vec3 norm;

    {{> fragment/normal.shader}}

    vec3 viewDir = normalize(viewPos - FragPos);
    //vec3 ambient = CalcChannel(material.ambient);
    //vec3 diffuse = CalcChannel(material.diffuse);
    //vec3 specular = CalcChannel(material.specular);
    {{{__SHADOW_FRAG_MAIN__}}}

    
    vec3 result = pointLightResolve(FragPos, norm, viewDir, shadow);
    //vec3 result = ambient + diffuse + specular;
   
    FragColor = vec4(result, 1);
    //FragColor = vec4(vec3(shadow), 1);
}