uniform sampler2D u_CloudNoiseTexture;
uniform sampler2D u_BlueNoiseTexture;
uniform sampler2D u_GrayNoiseTexture;
uniform float u_Time;
uniform int u_Frame;


/*
Real time PBR Volumetric Clouds by robobo1221.
Single scattering
Also includes volumetric light.
http://shadertoy.com/user/robobo1221

First ever somewhat PBR effect I decided to work on.
It uses the same algorithm to calculate the worldPosition as in: https://www.shadertoy.com/view/lstfR7

Feel free to fork and edit it. (Credit me please.)
Hope you enjoy!
*/

#define VOLUMETRIC_LIGHT
//#define SPHERICAL_PROJECTION

#define COVERAGE		.5
#define WIND			vec3(0, 0, -u_Time * .06)
#define cloudSpeed 0.02
#define cloudHeight 1600.0
#define cloudThickness 140.0
#define cloudDensity 0.02

#define fogDensity 0.00003

#define volumetricCloudSteps 16			//Higher is a better result with rendering of clouds.
#define volumetricLightSteps 8			//Higher is a better result with rendering of volumetric light.

#define cloudShadowingSteps 4			//Higher is a better result with shading on clouds.
#define volumetricLightShadowSteps 4	//Higher is a better result with shading on volumetric light from clouds

#define FBM_FREQ		2.76434

const float sunBrightness = 0.5;

#define earthRadius 6371000.0

//////////////////////////////////////////////////////////////////

float bayer2(vec2 a){
    a = floor(a);
    return fract( dot(a, vec2(.5, a.y * .75)) );
}

vec2 rsi(vec3 position, vec3 direction, float radius) {
    float PoD = dot(position, direction);
    float radiusSquared = radius * radius;

    float delta = PoD * PoD + radiusSquared - dot(position, position);
    if (delta < 0.0) return vec2(-1.0);
          delta = sqrt(delta);

    return -PoD + vec2(-delta, delta);
}

#define bayer4(a)   (bayer2( .5*(a))*.25+bayer2(a))
#define bayer8(a)   (bayer4( .5*(a))*.25+bayer2(a))
#define bayer16(a)  (bayer8( .5*(a))*.25+bayer2(a))
#define bayer32(a)  (bayer16(.5*(a))*.25+bayer2(a))
#define bayer64(a)  (bayer32(.5*(a))*.25+bayer2(a))
#define bayer128(a) (bayer64(.5*(a))*.25+bayer2(a))

//////////////////////////////////////////////////////////////////

#define cloudMinHeight cloudHeight
#define cloudMaxHeight (cloudThickness + cloudMinHeight)

#define sunPosition vec3(1.0, 1.0, 0.0)
#define PI 3.1415926535898
const float pi = PI;
const float rPi = 1.0 / pi;
const float hPi = pi * 0.5;
const float tau = pi * 2.0;
const float rLOG2 = 1.0 / log(2.0);


float noise_iq(vec3 x){
	vec3 p = floor(x);
	vec3 f = fract(x);
	f = f*f*(3.0 - 2.0*f);
	vec2 uv = (p.xy + vec2(37.0, 17.0)*p.z) + f.xy;
	vec2 rg = textureLod( u_CloudNoiseTexture, (uv+.5)/256., 0.).yx;
	return mix(rg.x, rg.y, f.z);
}
float fbm(
	vec3 pos,
	float lacunarity,
	bool lowres
){
	vec3 p = pos;
	float
	t  = 0.51749673 * noise_iq(p); p *= lacunarity;
	t += 0.25584929 * noise_iq(p); p *= lacunarity;
	t += 0.12527603 * noise_iq(p); p *= lacunarity;

	t += 0.06255931 * noise_iq(p); p *= lacunarity;

	if (lowres)
	{
		return t;
	}
	t += 0.031279655 * noise_iq(p); p *= lacunarity;
	t+= 0.0156398275 * noise_iq(p);
	
	return t;
}



mat3 rotationMatrix(vec3 axis, float angle)
{
    axis = normalize(axis);
    float s = sin(angle);
    float c = cos(angle);
    float oc = 1.0 - c;
    
    float xx = axis.x * axis.x;
    float yy = axis.y * axis.y;
    float zz = axis.z * axis.z;
    
    float xy = axis.x * axis.y;
    float xz = axis.x * axis.z;
    float zy = axis.z * axis.y;
    
    return mat3(oc * xx + c, oc * xy - axis.z * s, oc * xz + axis.y * s,
                oc * xy + axis.z * s, oc * yy + c, oc * zy - axis.x * s, 
                oc * xz - axis.y * s, oc * zy + axis.x * s, oc * zz + c);
}

vec3 sphereToCart(vec3 sphere) {
    vec2 c = cos(sphere.xy);
    vec2 s = sin(sphere.xy);
    
    return sphere.z * vec3(c.x * c.y, s.y, s.x * c.y);
}

vec3 calculateWorldSpacePosition(vec2 p)
{
	p = p * 2.0 - 1.0;
    
    vec3 worldSpacePosition =  vec3(p.x, p.y, 1.0);
    
    #ifdef SPHERICAL_PROJECTION
		worldSpacePosition = sphereToCart(worldSpacePosition * vec3(pi, hPi, 1.0));
	#endif
    
    return worldSpacePosition;
}


///////////////////////////////////////////////////////////////////////////////////

#define d0(x) (abs(x) + 1e-8)
#define d02(x) (abs(x) + 1e-3)


vec3 scatter(vec3 coeff, float depth){
	return coeff * depth;
}

vec3 absorb(vec3 coeff, float depth){
	return exp2(scatter(coeff, -depth));
}

float calcParticleThickness(float depth){
   	
    depth = depth * 2.0;
    depth = max(depth + 0.01, 0.01);
    depth = 1.0 / depth;
    
	return 100000.0 * depth;   
}



float calcParticleThicknessH(float depth){
   	
    depth = depth * 2.0 + 0.1;
    depth = max(depth + 0.01, 0.01);
    depth = 1.0 / depth;
    
	return 100000.0 * depth;   
}

float calcParticleThicknessConst(const float depth){
    
	return 100000.0 / max(depth * 2.0 - 0.01, 0.01);   
}

float rayleighPhase(float x){
	return 0.375 * (1.0 + x*x);
}

float hgPhase(float x, float g)
{
    float g2 = g*g;
	return 0.25 * ((1.0 - g2) * pow(1.0 + g2 - 2.0*g*x, -1.5));
}

float miePhaseSky(float x, float depth)
{
 	return hgPhase(x, exp2(-0.000003 * depth));
}

float powder(float od)
{
	return 1.0 - exp2(-od * 2.0);
}

float calculateScatterIntergral(float opticalDepth, float coeff){
    float a = -coeff * rLOG2;
    float b = -1.0 / coeff;
    float c =  1.0 / coeff;

    return exp2(a * opticalDepth) * b + c;
}

vec3 calculateScatterIntergral(float opticalDepth, vec3 coeff){
    vec3 a = -coeff * rLOG2;
    vec3 b = -1.0 / coeff;
    vec3 c =  1.0 / coeff;

    return exp2(a * opticalDepth) * b + c;
}



float Get3DNoise(vec3 pos) 
{
    float p = floor(pos.z);
    float f = pos.z - p;
    
    const float invNoiseRes = 1.0 / 256.0;
    
    float zStretch = 17.0 * invNoiseRes;
    
    vec2 coord = pos.xy * invNoiseRes + (p * zStretch);
    
    vec2 noise = vec2(texture(u_GrayNoiseTexture, coord).x,
					  texture(u_GrayNoiseTexture, coord + zStretch).x);
    
    return mix(noise.x, noise.y, f);
}

float density(vec3 pos,vec3 offset,bool lowres
){
	// signal
	if (pos.y < 0)
	{
		return 0;
	}
	if (pos.y == 0)
	{
		return 1.0;
	}
	vec3 p = pos * .002212242 + offset;
	float dens = fbm(p, FBM_FREQ, lowres);
	
	float cov = 1. - COVERAGE;
	dens *= smoothstep (cov, cov + .05, dens);


	return clamp(dens, 0., 1.);	
}

float getClouds(vec3 p, bool lowres)
{
    //p = vec3(p.x, length(p + vec3(0.0, earthRadius, 0.0)) - earthRadius, p.z);
    
    if (p.y < cloudMinHeight || p.y > cloudMaxHeight)
        return 0.0;
    
    float time = u_Time* cloudSpeed;
    vec3 movement = vec3(time, 0.0, time);
    
    vec3 cloudCoord = (p * 0.001) + WIND;
    
	//   float noise = Get3DNoise(cloudCoord) * 0.5;
    //  	  noise += Get3DNoise(cloudCoord * 2.0 + movement) * 0.25;
    //   	  noise += Get3DNoise(cloudCoord * 7.0 - movement) * 0.125;
    //   	  noise += Get3DNoise((cloudCoord + movement) * 16.0) * 0.0625;
    //       noise += Get3DNoise((cloudCoord + movement) * 32.0) * 0.03125;
    float noise = fbm(cloudCoord, FBM_FREQ, lowres);
    
    const float top = 0.004;
    const float bottom = 0.01;
    
    float horizonHeight = p.y - cloudMinHeight;
    float treshHold = (1.0 - exp2(-bottom * horizonHeight)) * exp2(-top * horizonHeight);
    float cov = 1. - COVERAGE; 
    noise *= smoothstep (cov, cov+0.1, noise);
    noise *= treshHold;
    
    return noise;
}

float getCloudShadow(vec3 p, vec3 sun_direction)
{
	const int steps = volumetricLightShadowSteps;
    float rSteps = cloudThickness / float(steps) / abs(sun_direction.y);
    
    vec3 increment = sun_direction* rSteps;
    vec3 position = sun_direction* (cloudMinHeight - p.y) / sun_direction.y + p;
    
    float transmittance = 0.0;
    
    for (int i = 0; i < steps; i++, position += increment)
    {
		transmittance += getClouds(position, true);
    }
    
    return exp2(-transmittance * rSteps);
} 




float getSunVisibility(vec3 p, vec3 sun_direction)
{
	const int steps = cloudShadowingSteps;
    const float rSteps = cloudThickness / float(steps);
    
    vec3 increment = sun_direction * rSteps;
    vec3 position = increment * 0.5 + p;
    
    float transmittance = 0.0;
    
    for (int i = 0; i < steps; i++, position += increment)
    {
		transmittance += getClouds(position, false);
    }
    
    return exp2(-transmittance * rSteps);
}

float phase2Lobes(float x)
{
    const float m = 0.6;
    const float gm = 0.8;
    
	float lobe1 = hgPhase(x, 0.8 * gm);
    float lobe2 = hgPhase(x, -0.5 * gm);
    
    return mix(lobe2, lobe1, m);
}


float getHeightFogOD(float height)
{
	const float falloff = 0.001;
    
    return exp2(-height * falloff) * fogDensity;
}
vec3 getVolumetricLightScattering(float opticalDepth, float phase, vec3 p, vec3 sunColor, vec3 skyLight, vec3 sun_direction)
{
    float intergal = calculateScatterIntergral(opticalDepth, 1.11);
    
	vec3 sunlighting = sunColor * phase * hPi * sunBrightness;
         sunlighting *= getCloudShadow(p, sun_direction);
    
    return (sunlighting  ) * intergal * pi;
}

vec3 calculateVolumetricLight(vec3 view_direction, vec3 color, float dither, vec3 sunColor, vec3 sun_direction, vec3 sky_color)
{
    
	const int steps = volumetricLightSteps;
    const float iSteps = 1.0 / float(steps);
    
    vec3 increment = view_direction * cloudMinHeight / clamp(view_direction.y, 0.1, 1.0) * iSteps;
    vec3 rayPosition = increment * dither;
    
    float stepLength = length(increment);
    
    vec3 scattering = vec3(0.0);
    vec3 transmittance = vec3(1.0);
    
    float lDotW = dot(sun_direction, view_direction);
    float phase = hgPhase(lDotW, 0.8);
    
    
    float shadow = 0.0f;
    for (int i = 0; i < steps; i++, rayPosition += increment)
    {
        float opticalDepth = getHeightFogOD(rayPosition.y) * stepLength;
        
        if (opticalDepth <= 0.0)
            continue;
        
		scattering += getVolumetricLightScattering(opticalDepth, phase, rayPosition, sunColor, sky_color, sun_direction) * transmittance;
        transmittance *= exp2(-opticalDepth);
    }
    
    return color * transmittance + scattering;
}

vec3 robobo1221Tonemap(vec3 color)
{
    #define rTOperator(x) (x / sqrt(x*x+1.0))

    float l = length(color);

    color = mix(color, color * 0.5, l / (l+1.0));
    color = rTOperator(color);

    return color;
}


vec3 getVolumetricCloudsScattering(float opticalDepth, float phase, vec3 p, vec3 sunColor, vec3 skyLight, vec3 sun_direction)
{
    float intergal = calculateScatterIntergral(opticalDepth, 1.11);
    
    float beersPowder = powder(opticalDepth * log(2.0));
    
	vec3 sunlighting = (sunColor * getSunVisibility(p, sun_direction) * beersPowder) * phase * hPi * sunBrightness;
    vec3 skylighting = sqrt(skyLight) * rPi ;
    
    return (sunlighting + skylighting) * intergal * pi;
}



vec3 calculateVolumetricClouds(vec3 view_direction, vec3 sun_direction, vec3 color, float dither, vec3 sunColor, vec3 skyColor)
{
	const int steps = volumetricCloudSteps;
    const float iSteps = 1.0 / float(steps);
    
    //if (pos.worldVector.y < 0.0)
     //   return color;
    
    float bottomSphere = rsi(vec3(0.0, 1.0, 0.0) * earthRadius, view_direction, earthRadius + cloudMinHeight).y;
    float topSphere = rsi(vec3(0.0, 1.0, 0.0) * earthRadius, view_direction, earthRadius + cloudMaxHeight).y;
    
    vec3 startPosition = view_direction * bottomSphere;
    vec3 endPosition = view_direction * topSphere;
    
    vec3 increment = (endPosition - startPosition) * iSteps;
    vec3 cloudPosition = increment * dither + startPosition;
    
    float stepLength = length(increment);
    
    vec3 scattering = vec3(0.0);
    float transmittance = 1.0;
    
    float lDotW = dot(sun_direction, view_direction);
    float phase = phase2Lobes(lDotW);
    
    
    for (int i = 0; i < steps; i++, cloudPosition += increment)
    {
        float opticalDepth = getClouds(cloudPosition, false) * stepLength;
        //float opticalDepth = density(cloudPosition, WIND,) * stepLength;
        
        if (opticalDepth <= 0.001)
            continue;
        
		scattering += getVolumetricCloudsScattering(opticalDepth, phase, cloudPosition, sunColor, skyColor, sun_direction) * transmittance;
        transmittance *= exp2(-opticalDepth);
    }
    
    return mix(color * transmittance + scattering, color, clamp(length(startPosition) * 0.00001, 0.0, 1.0));
   // return sunColor* transmittance;//color * transmittance + scattering;
}


vec3 GetClouds(vec3 view_direction, vec3 sun_direction, vec3 sky_color, vec3 sun_color, vec3 sky_irr)
{
    float dither = bayer8(mod(gl_FragCoord.yx, 32));
    vec3 lightAbsorb = vec3(0.0);
    vec3 color = vec3(0);
    color = calculateVolumetricClouds(view_direction, sun_direction, sky_color, dither, sun_color, sky_irr);
    color = calculateVolumetricLight(view_direction, color, dither, sun_color, sun_direction, sky_color); //vec3 calculateVolumetricLight(vec3 view_direction, vec3 color, float dither, vec3 sunColor, vec3 sun_direction)
    color = robobo1221Tonemap(color);
    return pow(color, vec3(1/2.2));
}
/*
void mainImage( out vec4 fragColor, in vec2 fragCoord )
{
    gatherPositions(pos, fragCoord, iMouse.xy, iResolution.xy);
    
    float dither = bayer16(fragCoord);
    
    vec3 lightAbsorb = vec3(0.0);

    vec3 color = vec3(0.0);
         color = calcAtmosphericScatter(pos, lightAbsorb);
         color = calculateVolumetricClouds(pos, color, dither, lightAbsorb);
         color = calculateVolumetricLight(pos, color, dither, lightAbsorb);
         color = pow(color, vec3(1.0 / 2.2));
         color = robobo1221Tonemap(color);

    fragColor = vec4(color, 1.0);
} */