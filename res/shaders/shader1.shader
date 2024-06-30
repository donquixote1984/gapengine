#shader vertex
#version 410
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 a_Color;
layout (location = 2) in vec2 a_TexCoord;
layout (location = 3) in float a_TexIndex;

out vec4 v_Color;
out vec2 v_TexCoord;
out float v_TexIndex;
void main()
{
    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
    v_Color = a_Color;
    v_TexCoord = a_TexCoord;
    v_TexIndex = a_TexIndex;
}
	
#shader fragment
#version 410
in vec4 v_Color;
in vec2 v_TexCoord;
in float v_TexIndex;
out vec4 FragColor;

uniform sampler2D u_Textures[2];
void main()
{
    int index = int(v_TexIndex);
    FragColor = texture(u_Textures[index], v_TexCoord);
}