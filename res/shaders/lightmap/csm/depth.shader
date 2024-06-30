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
// csm depth.shader
uniform mat4 model;
uniform int instancing;

// also need to consider about animation

uniform int u_IsPlaying;
{{__BONE_DEF__}}

void main()
{
    vec4 pos =  vec4(aPos.x, aPos.y, aPos.z, 1.0);
    vec3 norm = aNormal;
    
    {{__BONE_CALC__}}
    if (instancing == 1)
    {
        gl_Position =  model  {{__POSITION_MULTIPLIER__}}  * pos;
    } else {
        gl_Position =  model * pos;
    }
   
}
	
#include "depth.geometry.shader"


#shader fragment
#version 410
void main()
{             
    // gl_FragDepth = gl_FragCoord.z;
    //gl_FragDepth=0.5;
}