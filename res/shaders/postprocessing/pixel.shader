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
    vec2 offsets[25] = vec2[](
        vec2(-2*offset, 2*offset), vec2(-offset, 2 *offset), vec2(0, 2 * offset), vec2(offset, 2*offset), vec2(2 * offset, 2 * offset),
        vec2(-2*offset, offset), vec2(-offset, offset), vec2(0, offset), vec2(offset, offset), vec2(2 * offset, offset),
        vec2(-2*offset, 0), vec2(-offset, 0), vec2(0, 0), vec2(offset, 0), vec2(2 * offset, 0),
        vec2(-2*offset, -offset), vec2(-offset, -offset), vec2(0, -offset), vec2(offset, -offset), vec2(2 * offset, -offset),
        vec2(-2*offset, -2*offset), vec2(-offset, -2 *offset), vec2(0, -2 * offset), vec2(offset, -2*offset), vec2(2 * offset, -2 * offset)
    );

    vec3 sampleTex[25];
    for (int i = 0 ; i < 25; i++)
    {
        sampleTex[i] = vec3(texture(screenTexture, v_TexCoord.st + offsets[i]));
    }

    vec3 col = vec3(texture(screenTexture, floor(v_TexCoord.st/pixel)*pixel));

    for (int i =0; i < 25; i++)
    {
      //  col = sampleTex[i];
    }
    //col /= 25;
    FragColor = vec4(col, 1.0);
    //FragColor = vec4(1, 1, 0, 1);
}