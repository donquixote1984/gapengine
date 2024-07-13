

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

in vec2 TexCoords;
in vec2 FragPos;

uniform int u_Frequency;
uniform int u_Octaves;
uniform vec2 u_Res;
uniform float u_Amplitude;
uniform vec2 u_Offset;



vec2 grad( ivec2 z )  // replace this anything that returns a random vector
{
    // 2D to 1D  (feel free to replace by some other)
    int n = z.x+z.y*11111;

    // Hugo Elias hash (feel free to replace by another one)
    n = (n<<13)^n;
    n = (n*(n*n*15731+789221)+1376312589)>>16;

#if 0

    // simple random vectors
    return vec2(cos(float(n)),sin(float(n)));
    
#else

    // Perlin style vectors
    n &= 7;
    vec2 gr = vec2(n&1,n>>1)*2.0-1.0;
    return ( n>=6 ) ? vec2(0.0,gr.x) : 
           ( n>=4 ) ? vec2(gr.x,0.0) :
                              gr;
#endif                              
}

float noise( in vec2 p )
{
    ivec2 i = ivec2(floor( p ));
     vec2 f =       fract( p );
	
	vec2 u = f*f*(3.0-2.0*f); // feel free to replace by a quintic smoothstep instead

    return mix( mix( dot( grad( i+ivec2(0,0) ), f-vec2(0.0,0.0) ), 
                     dot( grad( i+ivec2(1,0) ), f-vec2(1.0,0.0) ), u.x),
                mix( dot( grad( i+ivec2(0,1) ), f-vec2(0.0,1.0) ), 
                     dot( grad( i+ivec2(1,1) ), f-vec2(1.0,1.0) ), u.x), u.y);
}

// -----------------------------------------------
float to01(float raw) 
{
    return (raw + 1) / 2;
}


out vec4 FragColor;
void main()
{
        float x = to01(FragPos.x) * u_Res.x ;
    float y = to01(FragPos.y) * u_Res.y ;


	vec2 uv = vec2(x,y )  *vec2(u_Res.x/u_Res.y,1.0) ;
	
	float f = 0.0;
		uv *= 10;
        mat2 m = mat2( 1.6,  1.2, -1.2,  1.6 );
		f  = 0.5000*noise( uv ); uv = m*uv;
		f += 0.2500*noise( uv ); uv = m*uv;
		f += 0.1250*noise( uv ); uv = m*uv;
		f += 0.0625*noise( uv ); uv = m*uv;
	f = 0.5 + 0.5*f;
	
	
	FragColor = vec4( f, f, f, 1.0 );
}