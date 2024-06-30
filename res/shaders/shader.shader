#shader vertex
#version 410
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 texCoord;

out vec2 v_TexCoord;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(aPos.x, aPos.y, aPos.z, 1.0);
    v_TexCoord = texCoord;
}
	
#shader fragment
#version 410
uniform vec4 u_Color;
uniform sampler2D u_Texture1;
uniform sampler2D u_Texture2;
in vec2 v_TexCoord;
out vec4 FragColor;
void main()
{
    vec4 texColor = mix(texture(u_Texture1, v_TexCoord), texture(u_Texture2, v_TexCoord), .5);
    FragColor = texColor;
}