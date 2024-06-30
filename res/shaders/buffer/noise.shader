#define M1 1597334677U     //1719413*929
#define M2 3812015801U     //140473*2467*11

#define K1 0.366025404 // simplex
#define K2 0.211324865 // simplex

#define HASHSCALE3 vec3(.1031, .1030, .0973)

uniform vec2 u_Res;
uniform float u_Amplitude;
uniform vec2 u_Offset;

uniform int perm[512];

float remap(float num , float sourceRange, float targetRange)
{
    return (num / sourceRange) * targetRange;
}

float fade(float delta)
{
    return 6.0 * pow(delta, 5.0) - 15.0 * pow(delta, 4.0) + 10.0 * pow(delta, 3.0);
}

float remap(float value, float min1, float max1, float min2, float max2) {
  return min2 + (value - min1) * (max2 - min2) / (max1 - min1);
}

float to01(float raw) 
{
    return (raw + 1) / 2;
}
float hash21(uvec2 q)
{
    q *= uvec2(M1, M2); 
    uint n = (q.x ^ q.y) * M1;
    return float(n) * (1.0/float(0xffffffffU));
}

vec2 hash22(vec2 coord)
{
    coord = vec2( dot(coord,vec2(127.1,311.7)),
              dot(coord,vec2(269.5,183.3)));

    return -1.0 + 2.0 * fract(sin(coord)*43758.5453123);
}

int incx(int x)
{
    return x >= u_Res.x ? 0 : x + 1;
}

int incy(int y)
{
    return y >= u_Res.y ? 0 : y + 1;
}
float grad2(int hash, float x, float y)
{
    switch (hash & 3)
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
            return 0.0;
    }
}
void hash2i4i(int x, int y,  out int h1, out int h2, out int h3, out int h4)
{
    x=x%512;
    y=y%512;
    h1 = perm[perm[    x]+    y];
    h2 = perm[perm[incx(x)]+    y];
    h3 = perm[perm[    x]+ incy(y)];
    h4 = perm[perm[incx(x)]+incy(y)];
    //h1 = perm[v.x];
}