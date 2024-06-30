uniform sampler2D u_CloudNoiseTexture;
uniform sampler2D u_BlueNoiseTexture;
uniform float u_Time;

#define COVERAGE		.5
#define THICKNESS		5.
#define ABSORPTION		1.030725
#define WIND			vec3(0, 0, -u_Time * .06)

#define FBM_FREQ		2.76434
#define NOISE_VALUE
#define FAKE_LIGHT
#define SUN_DIR			normalize(vec3(0, abs(sin(u_Time * .3)), -1))
#define PI 3.14159265359
#define BIAS 1e-4
#define STEPS			24
#define max_dist 1e8
#define MAX_STEPS_LIGHTS 6
#define SCATTERING_ANISO 0.3
const float rLOG2 = 1.0 / log(2.0);
const float rPi = 1.0 / PI;
const float hPi = PI * 0.5;
#define ABSORPTION_COEFFICIENT 0.9
struct sun_t {
	vec3 irradiance;
	vec3 direction;
};
struct ray_t {
	vec3 origin;
	vec3 direction;
};

struct sphere_t {
	vec3 origin;
	float radius;
	int material;
};

struct plane_t {
	vec3 direction;
	float distance;
};

struct hit_t {
	float t;
	int material_id;
	vec3 normal;
	vec3 origin;
};

const hit_t no_hit = hit_t(float(max_dist + 1e1), -1, vec3(0., 0., 0.), vec3(0., 0., 0.));
const vec3 sun_color = vec3(1., .2, .55);
const plane_t ground = plane_t(vec3(0, -1, 0), 0);



// noise iq

float hash(float n){
	return fract(sin(n)*753.5453123);
}

float noise_iq(vec3 x){
	vec3 p = floor(x);
	vec3 f = fract(x);
	f = f*f*(3.0 - 2.0*f);
	vec2 uv = (p.xy + vec2(37.0, 17.0)*p.z) + f.xy;
	vec2 rg = textureLod( u_CloudNoiseTexture, (uv+.5)/256., 0.).yx;
	return mix(rg.x, rg.y, f.z);
}


//

// noise w

vec3 hash_w(vec3 x){
	return texture(u_CloudNoiseTexture, (x.xy + vec2(3.0, 1.0)*x.z + 0.5) / 256.0, -100.0).xyz;
}
vec3 noise_w(vec3 x){
	vec3 p = floor(x);
	vec3 f = fract(x);

	float id = 0.0;
	vec2 res = vec2(100.0, 100.0);
	for (int k = -1; k <= 1; k++)
		for (int j = -1; j <= 1; j++)
			for (int i = -1; i <= 1; i++)
			{
				vec3 b = vec3(float(i), float(j), float(k));
				vec3 r = vec3(b) - f + hash_w(p + b);
				float d = dot(r, r);

				if (d < res.x)
				{
					id = dot(p + b, vec3(1.0, 57.0, 113.0));
					res = vec2(d, res.x);
				}
				else if (d < res.y)
				{
					res.y = d;
				}
			}

	return vec3(sqrt(res), abs(id));
}

//

#ifdef NOISE_VALUE
#define noise(x) noise_iq(x)
#endif
#ifdef NOISE_WORLEY
#define noise(x) (1. - noise_w(x).r)
//#define noise(x) abs( noise_iq(x / 8.) - (1. - (noise_w(x * 2.).r)))
#endif
#ifdef NOISE_PERLIN
#define noise(x) abs(cnoise(x))
#endif



float fbm(
	vec3 pos,
	float lacunarity,
	bool lowres
){
	vec3 p = pos;
	float
	t  = 0.51749673 * noise(p); p *= lacunarity;
	t += 0.25584929 * noise(p); p *= lacunarity;
	t += 0.12527603 * noise(p); p *= lacunarity;

	t += 0.06255931 * noise(p); p *= lacunarity;

	if (lowres)
	{
		return t;
	}
	t += 0.031279655 * noise(p); p *= lacunarity;
	t+= 0.0156398275 * noise(p);
	
	return t;
}

bool intersect_plane(ray_t ray,plane_t p){
	float denom = dot(p.direction, ray.direction);
	if (denom < 1e-6) return false;

	vec3 P0 = vec3(p.distance, p.distance, p.distance);
	float t = dot(P0 - ray.origin, p.direction) / denom;
	if (t < 0. || t > max_dist) return false;
	
	return true;
}

void raySphereIntersect(ray_t ray, float radius, out hit_t hit)
{
    // note to future me: don't need "a" bcuz rd is normalized and dot(rd, rd) = 1
 	float b = 2. * dot(ray.origin, ray.direction);
    float c = dot(ray.origin, ray.origin) - radius * radius;
    float d = sqrt(b * b - 4. * c);

    float t0 = (-b + d) * .5;
	hit.t = t0;
	hit.origin = ray.origin + ray.direction * t0;
}

void intersect_sphere(
	ray_t ray,
	sphere_t sphere,
	out hit_t hit
){
	
	vec3 rc = sphere.origin - ray.origin;
	float radius2 = sphere.radius * sphere.radius;
	float tca = dot(rc, ray.direction);
//	if (tca < 0.) return;

	float d2 = dot(rc, rc) - tca * tca;
	if (d2 > radius2)
		return;

	float thc = sqrt(radius2 - d2);
	float t0 = tca - thc;
	float t1 = tca + thc;

	if (t0 < 0.) t0 = t1;
	if (t0 > hit.t)
		return;

	vec3 impact = ray.origin + ray.direction * t0;
	if (impact.y < 0)
	{
		return;
	}

	hit.t = t0;
	hit.material_id = sphere.material;
	hit.origin = impact;
	hit.normal = (impact - sphere.origin) / sphere.radius;
}


float BeersLaw (float dist, float absorption) {
  return exp(-dist * absorption);
}



float density(
	vec3 pos,
	vec3 offset,
	bool lowres
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

float HenyeyGreenstein(float g, float mu) {
  float gg = g * g;
	return (1.0 / (4.0 * PI))  * ((1.0 - gg) / pow(1.0 + gg - 2.0 * g * mu, 1.5));
}

vec3 robobo1221Tonemap(vec3 color)
{
    #define rTOperator(x) (x / sqrt(x*x+1.0))

    float l = length(color);

    color = mix(color, color * 0.5, l / (l+1.0));
    color = rTOperator(color);

    return color;
}

float lightmarch(vec3 position, ray_t ray, sun_t sun) {
  vec3 lightDirection = normalize(sun.direction);
  float totalDensity = 0.0;
  float marchSize = 0.1 * (texture(u_BlueNoiseTexture, ( ray.direction.yx)/1024.0).r);
  for (int step = 0; step < MAX_STEPS_LIGHTS; step++) {
      position += lightDirection * marchSize * float(step);
      float lightSample = density(position, WIND, false);
      totalDensity += lightSample;
  }

  float transmittance = BeersLaw(totalDensity, ABSORPTION_COEFFICIENT);
  return transmittance;
}


float calculateScatterIntergral(float density, float coeff)
{
	float a = -coeff * rLOG2;
    float b = -1.0 / coeff;
    float c =  1.0 / coeff;

    return exp2(a * density) * b + c;
}
float powder(float od)
{
	return 1.0 - exp2(-od * 2.0);
}



float render_clouds(ray_t eye, sun_t sun, out float alpha){



	hit_t hit = no_hit;
	sphere_t atmosphere = sphere_t(earth_center, 7000, 0);
	intersect_sphere(eye, atmosphere, hit);
	//hit_t hit_2 = no_hit;
	//intersect_sphere(eye, atmosphere_2, hit_2);

	const float thickness = THICKNESS; // length(hit_2.origin - hit.origin);
	//const float r = 1. - ((atmosphere_2.radius - atmosphere.radius) / thickness);
	const int steps = STEPS; // +int(32. * r);
	float march_step = thickness  *  texture(u_BlueNoiseTexture, (eye.direction.yx)/1024.0).r;;

	vec3 dir_step = eye.direction * march_step;
	vec3 pos = //eye.origin + eye.direction * 100.; 
		hit.origin;

	float T = 1.; // transmitance
	vec3 C = vec3(0, 0, 0); // color
	alpha = 0.0;
	float totalTransmittance = 1.0;
	float lightEnergy = 0.0;

	float phase = HenyeyGreenstein(SCATTERING_ANISO, dot(eye.direction, sun.direction));

	for (int i = 0; i < steps; i++) {
		float h = float(i) / float(steps);
		float dens = density(pos, WIND, false);

		float T_i = exp(-ABSORPTION * dens * march_step);
		T *= T_i;
		if (T < .01) break;

		if (dens > 0.001)
		{
			float lightTransmittance = lightmarch(pos, eye, sun);
			float luminance = dens * phase + 0.1;
			totalTransmittance *= lightTransmittance;
      		lightEnergy += totalTransmittance * luminance;
		}

		alpha += (1. - T_i) * (1. - alpha);
		pos += dir_step;
		if (length(pos) > 1e5) break;
	}

	vec3 sky_irradiance;
  	vec3 sun_irradiance = GetSunAndSkyIrradiance(
       pos - earth_center, sun_direction, sun_direction, sky_irradiance);
	//float hacksundome = exp(1 * clamp(dot(normalize(pos - eye.origin), sun_direction), 0, 1));
	return pow(lightEnergy, 0.7);
}
