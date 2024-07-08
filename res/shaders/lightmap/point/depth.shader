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


uniform mat4 model;
uniform int instancing;
uniform int u_IsPlaying;
layout (std140) uniform Bones
{
    mat4 bonesMatrices[600];
};
{{__BONE_DEF__}}

void main()
{
    vec4 pos = vec4(aPos.x, aPos.y, aPos.z, 1.0);
    vec3 norm = aNormal;
    {{__BONE_CALC__}}

    if (instancing == 1) {
        gl_Position =  model  {{__POSITION_MULTIPLIER__}}  * pos;
    } else {
        gl_Position =  model * pos;
    }
}
	
#shader geometry
#version 410
layout (triangles) in;
layout (triangle_strip, max_vertices=108) out;
layout (std140) uniform PSM
{
    mat4 shadowCubeMatrices[60];
};

#define MAX_POINT_LIGHTS 10

uniform int u_PointLightNum;


struct PointLight {
    int index;
    vec3 position;
    float farPlane;
};
uniform PointLight u_PointLights[MAX_POINT_LIGHTS];

out vec4 FragPos; // FragPos from GS (output per emitvertex)
out vec3 lightPos;
out float farPlane;

void main()
{
    for (int pnum = 0; pnum < u_PointLightNum; pnum++) {
        for(int face = 0; face < 6; ++face)
        {
            gl_Layer = pnum * 6 + face; // built-in variable that specifies to which face we render.
            for(int i = 0; i < 3; ++i) // for each triangle's vertices
            {
                FragPos = gl_in[i].gl_Position;
                gl_Position = shadowCubeMatrices[pnum* 6 + face] * FragPos;
                lightPos = u_PointLights[pnum].position;
                farPlane = u_PointLights[pnum].farPlane;
                EmitVertex();
            }    
            EndPrimitive();
        }
    }
    
} 

#shader fragment
#version 410

in vec3 lightPos;
in vec4 FragPos;
in float farPlane;

void main()
{             
    float lightDistance = length(FragPos.xyz - lightPos);
    lightDistance = lightDistance / farPlane;
    gl_FragDepth = lightDistance;
    //gl_FragDepth = 0.3;
}