#shader vertex
#version 410

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;

out vec2 TexCoords;
out vec2 FragPos;

void main()
{
    gl_Position = vec4(aPos.x, aPos.y, 0.0, 1.0);
    TexCoords = aTexCoords;
    FragPos = aTexCoords;
}
	

#shader fragment
#version 410
in vec2 TexCoords;
out vec4 FragColor;
in vec2 FragPos;
void main()
{
   FragColor = vec4(vec3(TexCoords, 0), 1.0);
}

