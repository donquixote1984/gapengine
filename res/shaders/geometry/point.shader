#shader vertex
#version 410
layout (location = 0) in vec3 aPos;


layout (std140) uniform Matrices
{
    mat4 projection;
    mat4 view;
};

uniform mat4 model;

void main()
{
    gl_Position= projection * view * model* vec4(aPos.x, aPos.y, aPos.z, 1.0);
    //gl_Position=pos.xyww;
}
	
#shader fragment
#version 410
out vec4 FragColor;

void main()
{
    FragColor = vec4(vec3(1.0), 1.0);
}