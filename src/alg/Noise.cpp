#include "Noise.h"

float Noise::fract(float num)
{
    float frac;
    frac = modf(num, &frac);
    return frac;
}
float Noise::length (vec2f v)
{
    return sqrt(length2(v));
}
float Noise::length (float x, float y)
{
    return sqrt(length2(x, y));
}

float Noise::length2(vec2f v)
{
    return v.x * v.x + v.y + v.y;
}
float Noise::length2(float x, float y)
{
    return x * x + y * y;
}



vec2f Noise::hash22(vec2f num)
{
    float x = num.x;
    float y = num.y;
    vec2f ret;

    ret.x = x * 127.1 + y * 311.7;
    ret.y = x * 269.5 + y * 183.3;
    float sin0 = sin(ret.x) * 43758.5453123;
    float sin1 = sin(ret.y) * 43758.5453123;
    ret.x = (sin0 - floor(sin0)) * 2.0 - 1.0;
    ret.y = (sin1 - floor(sin1)) * 2.0 - 1.0;

    return normalize(ret);
}

float Noise::to01(float raw)
{
    return 2 * raw - 1;
}
float Noise::lerp(float u, float v, float t)
{
    return u * t + v * (1 - t);
}
int Noise::inc(int num)
{
    num+=1;
    if (num == xres)
    {
        num = 0;
    }
    return num;
}

vec2f Noise::normalize(vec2f &v)
{
    float len = length(v);
    v.x /= len;
    v.y /= len;
    return v;
}

float Noise::fitRange(float num , float sourceRange, float targetRange)
{
    return (num / sourceRange) * targetRange;
}

float Noise::fade(float delta)
{
    return 6.0 * pow(delta, 5.0) - 15.0 * pow(delta, 4.0) + 10.0 * pow(delta, 3.0);
}