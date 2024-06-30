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
    if (instancing == 1)
    {
        gl_Position =  model  {{__POSITION_MULTIPLIER__}}  * vec4(aPos, 1.0);
    } else {
        gl_Position =  model * vec4(aPos, 1.0);
    }
   
}
	
#include "sun.geometry.shader"


#shader fragment
#version 410
void main()
{             
    // gl_FragDepth = gl_FragCoord.z;
    //gl_FragDepth=0.5;
}