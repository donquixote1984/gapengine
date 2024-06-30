#shader vertex
#version 410
layout (location = 0) in vec3 aPos;

{{__A_LOCATION_1__}}
{{__A_LOCATION_2__}}
{{__A_LOCATION_3__}}

uniform mat4 model;
uniform int instancing;

void main()
{
    if (instancing == 1) {
        gl_Position =  model  {{__POSITION_MULTIPLIER__}}  * vec4(aPos, 1.0);
    } else {
        gl_Position =  model * vec4(aPos, 1.0);
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