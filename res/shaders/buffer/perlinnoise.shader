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

uniform int u_Frequency;
uniform int u_Octaves;

#include "noise.shader"


float noise2D(float x, float y, int frequency, float amp, vec2 offset)
{
    
    float xm = remap(x + offset.x, u_Res.x, frequency);
    float ym = remap(y + offset.y, u_Res.y, frequency);

    int xi = int(floor(xm));
    int yi = int(floor(ym));
    float xof = fract(u_Offset.x / u_Res.x);
    float yof = fract(u_Offset.y / u_Res.y);

    float u = fract(xm);
    float v = fract(ym);

    float xf = fade(u);
    float yf = fade(v);

    int h1, h2, h3, h4;
    hash2i4i(xi, yi, h1, h2, h3, h4);
    float g1 = grad2(h1, u, v);
    float g2 = grad2(h2, u-1, v);
    float g3 = grad2(h3, u, v-1);
    float g4 = grad2(h4, u-1, v-1);


    float lerp1 = mix(g1, g2, xf);
    float lerp2 = mix(g3, g4, xf);

    return mix(lerp1, lerp2, yf) * amp;
    //return h1;
}
float noise2DOctaves(float x, float y, int octaves)
{
    int frequency = u_Frequency;
    float amplitude = u_Amplitude;
    vec2 offset = u_Offset;
    float total = 0.0;
    float maxvalue = 0.0;
    for (int i =0; i < octaves; i++)
    {
        total += noise2D(x, y, frequency, amplitude, offset);
        frequency *= 2;
        amplitude /= 2;
        offset/=2;
    }
    return to01(total); 
}
void main() 
{

    float x = to01(FragPos.x) * u_Res.x ;
    float y = to01(FragPos.y) * u_Res.y ;

    vec3 noise = vec3(noise2DOctaves(x, y, u_Octaves));

    //FragColor = vec4(vec3(1), 1);
    FragColor = vec4(noise, 1);
}