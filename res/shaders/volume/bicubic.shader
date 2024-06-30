#shader vertex
#version 410

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;

layout (std140) uniform Matrices
{
    mat4 projection;
    mat4 view;
};
out vec2 TexCoords;
void main()
{
    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
    TexCoords = aTexCoords;
}
	
#shader fragment

#version 410
uniform sampler2D u_Texture;
uniform bool uUpscaling;
uniform vec2 uResolution;

in vec2 TexCoords;

float w0(float a)
{
    return (1.0/6.0)*(a*(a*(-a + 3.0) - 3.0) + 1.0);
}

float w1(float a)
{
    return (1.0/6.0)*(a*a*(3.0*a - 6.0) + 4.0);
}

float w2(float a)
{
    return (1.0/6.0)*(a*(a*(-3.0*a + 3.0) + 3.0) + 1.0);
}

float w3(float a)
{
    return (1.0/6.0)*(a*a*a);
}

// g0 and g1 are the two amplitude functions
float g0(float a)
{
    return w0(a) + w1(a);
}

float g1(float a)
{
    return w2(a) + w3(a);
}

// h0 and h1 are the two offset functions
float h0(float a)
{
    return -1.0 + w1(a) / (w0(a) + w1(a));
}

float h1(float a)
{
    return 1.0 + w3(a) / (w2(a) + w3(a));
}

vec4 texture_bicubic(sampler2D tex, vec2 uv, vec4 texelSize, vec2 fullSize, float lod)
{
	uv = uv*texelSize.zw + 0.5;
	vec2 iuv = floor( uv );
	vec2 fuv = fract( uv );

    float g0x = g0(fuv.x);
    float g1x = g1(fuv.x);
    float h0x = h0(fuv.x);
    float h1x = h1(fuv.x);
    float h0y = h0(fuv.y);
    float h1y = h1(fuv.y);

	vec2 p0 = (vec2(iuv.x + h0x, iuv.y + h0y) - 0.5) * texelSize.xy;
	vec2 p1 = (vec2(iuv.x + h1x, iuv.y + h0y) - 0.5) * texelSize.xy;
	vec2 p2 = (vec2(iuv.x + h0x, iuv.y + h1y) - 0.5) * texelSize.xy;
	vec2 p3 = (vec2(iuv.x + h1x, iuv.y + h1y) - 0.5) * texelSize.xy;
	
    vec2 lodFudge = pow(1.95, lod) / fullSize;
    return g0(fuv.y) * (g0x * 
        textureLod(tex, p0, lod)  +
                        
        g1x * textureLod(tex, p1, lod)
                    ) +
           g1(fuv.y) * (
            g0x * textureLod(tex, p2, lod)  +
                        g1x * textureLod(tex, p3, lod));
}


vec4 textureBicubic(sampler2D s, vec2 uv, float lod) {
 vec2 lodSizeFloor = vec2(textureSize(s, int(lod)));
 vec2 lodSizeCeil = vec2(textureSize(s, int(lod + 1.0)));
 vec2 fullSize = vec2(textureSize(s, 0));
 vec4 floorSample = texture_bicubic( s, uv, vec4(1.0 / lodSizeFloor.x, 1.0 / lodSizeFloor.y, lodSizeFloor.x, lodSizeFloor.y), fullSize, floor(lod));
 vec4 ceilSample = texture_bicubic( s, uv, vec4(1.0 / lodSizeCeil.x, 1.0 / lodSizeCeil.y, lodSizeCeil.x, lodSizeCeil.y), fullSize, ceil(lod));
 return mix(floorSample, ceilSample, fract(lod));
}

out vec4 FragColor;

void main() {
    vec2 uv = TexCoords;

    //vec4 color = res;
    FragColor = texture(u_Texture, uv);
}