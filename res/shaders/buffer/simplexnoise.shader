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

    vec2 coord = vec2(xm, ym);

    vec2 i = floor(coord + (coord.x + coord.y) * K1);
    vec2 a = coord - (i - (i.x + i.y) * K2);
    vec2 o = (a.x < a.y) ? vec2(0.0, 1.0) : vec2(1.0, 0.0);
    vec2 b = a - o + K2;
    vec2 c = a - 1.0 + 2.0 * K2;
    vec3 h = max(0.5 - vec3(dot(a, a), dot(b, b), dot(c, c)), 0.0);
    vec3 n = h * h * h * h * vec3(dot(a, hash22(i + offset)), dot(b, hash22(i + o + offset)), dot(c, hash22(i + 1.0 + offset)));
    return dot(vec3(70.0, 70.0, 70.0), n) * amp;
}
float noise2DOctaves(float x, float y, int octaves)
{
    int frequency = freq;
    float amplitude = amp;
    float total = 0.0;
    float maxvalue = 0.0;
    for (int i =0; i < octaves; i++)
    {
        total += noise2D(x, y, frequency, amplitude);
        frequency *= 2;
        amplitude /= 2;
    }
    return to01(total); 
}
void main() 
{

    float x = to01(FragPos.x) * resx;
    float y = to01(FragPos.y) * resy;

    vec3 noise = vec3(noise2DOctaves(x, y, octaves));

    //FragColor = vec4(vec3(1), 1);
    FragColor = vec4(noise, 1);
}