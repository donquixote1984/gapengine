#shader vertex
#version 410

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;

out vec2 TexCoords;
out vec2 FragPos;

void main()
{
    TexCoords = aTexCoords;
	gl_Position = vec4(aPos, 1.0);
    FragPos = gl_Position.xy;
}

#shader fragment 
#version 410

out vec4 FragColor;
in vec2 TexCoords;
in vec2 FragPos;

const float PI = 3.14159265359;

void main() 
{

    FragColor = vec4(vec3(1), 1);
}