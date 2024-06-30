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

uniform int freq;
uniform int octaves;

const float PI = 3.14159265359;

#include "noise.shader"


float noise2D(float x, float y, int frequency, float amp)
{
    float xm = remap(x, resx, frequency);
    float ym = remap(y, resy, frequency);

    vec2 uv =  vec2(xm, ym);
    vec2 pos_index = floor(uv);
    vec2 pos_fract = fract(uv);
    float min_dist = frequency * frequency;
    int range = 2;
    for (int i = -range; i <= range ; i++)
    {
        for (int j = -range; j <= range; j++)
        {
            vec2 offset_pos = pos_index + vec2(i, j);
            vec2 offset_hash = hash22(offset_pos);
            
            min_dist = min(min_dist, length(offset_hash + vec2(i, j) - pos_fract));
        }
    }

    return min_dist;

}

void main() 
{

    float x = to01(FragPos.x) * resx;
    float y = to01(FragPos.y) * resy;
    vec3 noise = vec3(noise2D(x, y, freq, amp));

    //FragColor = vec4(vec3(1), 1);
    FragColor = vec4(noise, 1);
}