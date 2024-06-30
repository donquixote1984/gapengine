#shader vertex
#version 410
layout (location = 0) in vec3 aPos;

{{__A_LOCATION_1__}}
{{__A_LOCATION_2__}}
{{__A_LOCATION_3__}}

uniform mat4 lightSpaceMatrix;
uniform mat4 model;
uniform int instancing;

void main()
{
    if (instancing == 1)
    {
        gl_Position = lightSpaceMatrix * model  {{__POSITION_MULTIPLIER__}}  * vec4(aPos, 1.0);
    } else {
        gl_Position = lightSpaceMatrix * model * vec4(aPos, 1.0);
    }
   
}
	
#shader fragment
#version 410
void main()
{             
    // gl_FragDepth = gl_FragCoord.z;
}