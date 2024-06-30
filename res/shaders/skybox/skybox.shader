#shader vertex
#version 410
layout (location = 0) in vec3 aPos;

out vec3 v_TexCoord;

uniform mat4 view;
uniform mat4 projection;

void main()
{
    vec4 pos= projection * view * vec4(aPos.x, aPos.y, aPos.z, 1.0);
    gl_Position=pos.xyww;
    v_TexCoord = aPos;
}
	
#shader fragment
#version 410
in vec3 v_TexCoord;
out vec4 FragColor;

uniform samplerCube skybox;

void main()
{
    vec3 color = texture(skybox, v_TexCoord).rgb;
    color = color / (color + vec3(1.0));
    color = pow(color, vec3(1.0/2.2)); 
    FragColor =  vec4(color, 1.0);
}