#pragma once
#include <math.h>
#include <cmath>

struct vec2f
{
    float x = .0f;
    float y = .0f;
};

struct vec3f
{
    float x = .0f;
    float y = .0f;
    float z = .0f;
};


class Noise
{
protected:
    float xres = 2048.0f;
    float yres = 2048.0f;
    int freq = 16;
    float amp = 1.0;
    int octave = 1;
    float fract(float);
    vec2f hash22(vec2f);
    float length(vec2f);
    float length(float x, float y);
    float length2(vec2f v);
    float length2(float x, float y);
    float fade(float delta);
    vec2f normalize(vec2f &v);
    float fitRange(float num, float sourceRange , float targetRange);
    float to01(float raw);
    int inc(int num);

    float lerp(float u, float v, float t);

public:
    virtual float Noise2D(int x, int y) = 0;
    virtual float Noise3D(int x, int y, int z) = 0;
};