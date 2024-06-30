#shader vertex
#version 410
layout (location = 0) in vec3 aPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(aPos.x, aPos.y, aPos.z, 1.0);
}
	
#shader fragment
#version 410

out vec4 FragColor;
void main()
{
    FragColor = vec4(0.2, 0.2, 0.2, 1.0);
}