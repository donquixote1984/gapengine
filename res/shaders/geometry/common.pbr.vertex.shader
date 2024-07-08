#shader vertex
#version 410


layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
{{__A_LOCATION_1__}}
{{__A_LOCATION_2__}}
{{__A_LOCATION_3__}}
{{__A_LOCATION_4__}}
{{__A_LOCATION_5__}}

layout (std140) uniform Matrices
{
    mat4 projection;
    mat4 view;
    mat4 clipPlane;
};
layout (std140) uniform Bones
{
    mat4 bonesMatrices[600];
};

uniform mat4 model;
uniform int u_IsPlaying;

{{> vertex/uniforms.h.shader}}
{{__SHADOW_VERT_HEADER__}}
{{__BONE_DEF__}}

out vec3 Normal;
out vec3 FragPos;
out vec3 FragViewPos;
out vec2 TexCoords;
out vec3 ScreenSpaceDir;

{{__A_OUT_1__}}
{{__A_OUT_2__}}
{{__A_OUT_3__}}

void main()
{
    vec4 pos =  vec4(aPos.x, aPos.y, aPos.z, 1.0);
    vec3 norm = aNormal;
    {{__BONE_CALC__}}
    gl_Position = projection * view * model {{__POSITION_MULTIPLIER__}} * pos;
    vec4 FragWorldPos = model {{__POSITION_MULTIPLIER__}} * vec4(pos.x, pos.y, pos.z, 1.0);
    FragPos = vec3(FragWorldPos);
    FragViewPos = vec3(view * model {{__POSITION_MULTIPLIER__}} * vec4(pos.x, pos.y, pos.z, 1.0));
    mat3 normalMatrix = mat3(transpose(inverse(model {{__POSITION_MULTIPLIER__}})));
    Normal = normalize(normalMatrix * norm);
    TexCoords = aTexCoords;
    // (inverse(model_from_view) * vec4((inverse(view_from_clip) * vec4(aPos.x, aPos.y, 0, 1.0)).xyz, 0.0)).xyz;
    ScreenSpaceDir =  (inverse(view) * vec4((inverse(projection) * vec4(pos.x, pos.y, 0, 1.0)).xyz, 0.0)).xyz;
    ScreenSpaceDir = normalize(ScreenSpaceDir);

    gl_ClipDistance[0] = dot(FragWorldPos, vec4(clipPlane));

    {{{__TBN__}}}
    {{{__SHADOW_VERT_MAIN__}}}
    {{{__INSTANCING_UV__}}}
}
	