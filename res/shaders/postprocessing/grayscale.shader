#shader vertex
#version 410
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 texCoord;

out vec2 v_TexCoord;

void main()
{
    gl_Position = vec4(aPos.x, aPos.y, 0.0, 1.0);
    v_TexCoord = texCoord;
}
	
#shader fragment
#version 410
uniform sampler2D screenTexture;
in vec2 v_TexCoord;
out vec4 FragColor;

const float offset = 1.0/200.0;
const float pixel = 0.005;
void main()
{
    FragColor = texture(screenTexture, v_TexCoord);
    float average = 0.2126 * FragColor.r + 0.7152 * FragColor.g + 0.0722 * FragColor.b;
    FragColor = vec4(average, average, average, 1.0);
}