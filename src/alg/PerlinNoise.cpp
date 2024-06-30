#include "PerlinNoise.h"
#include <cmath>

PerlinNoise::PerlinNoise()
{}

float PerlinNoise::Noise3D(int x, int y, int z)
{
    return 0; // unimplemented
}
float PerlinNoise::Noise2D(int x, int y)
{
    float xm = fitRange(x, xres, freq);
    float ym = fitRange(x, yres, freq);
    int x_index = floor(xm);
    int y_index = floor(ym);
    float u = fract(xm);
    float v = fract(ym);

    float x_fade = fade(u);
    float y_fade = fade(v);

    int h1, h2, h3, h4;
    hash2D(x_index, y_index, h1, h2, h3, h4);

    float g1 = grad2(h1, u, v);
    float g2 = grad2(h2, u-1, v);
    float g3 = grad2(h3, u-1, v-1);
    float g4 = grad2(h4, u, v-1);

    float lerp1 = lerp(g1, g2, u);
    float lerp2 = lerp(g3, g4, u);

    return to01(lerp(lerp1, lerp2, v));
}


void PerlinNoise::hash2D(int xi, int yi, int &h1, int &h2, int &h3, int &h4)
{
    h1 = perm[perm[    xi]+    yi];
    h2 = perm[perm[    xi]+ inc(yi)];
    h3 = perm[perm[inc(xi)]+    yi];
    h4 = perm[perm[inc(xi)]+inc(yi)];
}

float PerlinNoise::grad2(int hash, float x, float y)
{
    switch (hash & 15)
    {
        case 0:
            return x + y;
        case 1:
            return -x + y;
        case 2:
            return x - y;
        case 3:
            return -x - y;
        default:
            return 0;
    }
}

float PerlinNoise::grad3(int hash, float x, float y, float z)
{
    switch (hash & 15)
    {
        case 0:
            return x + y;
        case 1:
            return -x + y;
        case 2:
            return  x - y;
        case 3:
            return -x - y;
        case 4:
            return  x + z;
        case 5:
            return -x + z;
        case 6:
            return  x - z;
        case 7:
            return -x - z;
        case 8:
            return  y + z;
        case 9:
            return -y + z;
        case 10:
            return  y - z;
        case 11:
            return -y - z;
        case 12:
            return  y + x;
        case 13:
            return -y + z;
        case 14:
            return  y - x;
        case 15:
            return -y - z;
        return 0;
    }
    return 0;
}