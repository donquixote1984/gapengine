#shader fragment
#version 410


const float kLengthUnitInMeters = 1000.0;

uniform vec3 camera;
uniform float exposure;
uniform vec3 white_point;
uniform vec3 earth_center;
uniform vec3 sun_direction;
uniform float sun_holo_size;
uniform vec2 sun_size;
in vec3 view_ray;
out vec4 FragColor;




const vec3 kSphereCenter = vec3(0.0, 0.0, 1000.0) / kLengthUnitInMeters;
const float kSphereRadius = 1000.0 / kLengthUnitInMeters;
const vec3 kSphereAlbedo = vec3(0.8);
const vec3 kGroundAlbedo = vec3(0.0, 0.0, 0.04);

#ifdef USE_LUMINANCE
#define GetSolarRadiance GetSolarLuminance
#define GetSkyRadiance GetSkyLuminance
#define GetSkyRadianceToPoint GetSkyLuminanceToPoint
#define GetSunAndSkyIrradiance GetSunAndSkyIlluminance
#endif

vec3 GetSolarRadiance();
vec3 GetSolarLuminance();
vec3 GetSkyRadiance(vec3 camera, vec3 view_ray, float shadow_length,
    vec3 sun_direction, out vec3 transmittance);
vec3 GetSkyRadianceToPoint(vec3 camera, vec3 point, float shadow_length,
    vec3 sun_direction, out vec3 transmittance);
vec3 GetSunAndSkyIrradiance(
    vec3 p, vec3 normal, vec3 sun_direction, out vec3 sky_irradiance);


#include "cloud.functions2.shader"

float GetSunVisibility(vec3 point, vec3 sun_direction) {
  
  return 1.0;
}

float GetSkyVisibility(vec3 point) {

  return
      1.0 ;//+ p.z / sqrt(p_dot_p) * kSphereRadius * kSphereRadius / p_dot_p;
}

void main() {
   vec3 view_direction = normalize(view_ray);
  // Tangent of the angle subtended by this fragment.
  float fragment_angular_size =
      length(dFdx(view_ray) + dFdy(view_ray)) / length(view_ray);


  vec3 p = camera - earth_center;
  float p_dot_v = dot(p, view_direction);
  float p_dot_p = dot(p, p);
  float ray_earth_center_squared_distance = p_dot_p - p_dot_v * p_dot_v;
  float distance_to_intersection = -p_dot_v - sqrt(
      earth_center.y * earth_center.y - ray_earth_center_squared_distance);

  // Compute the radiance reflected by the ground, if the ray intersects it.
  float ground_alpha = 0.0;
  vec3 ground_radiance = vec3(0.0);


/**

    vec3 point = camera + view_direction * distance_to_intersection;
    vec3 normal = normalize(point - kSphereCenter);

    // Compute the radiance reflected by the sphere.
    vec3 sky_irradiance;
    vec3 sun_irradiance = GetSunAndSkyIrradiance(
        point - earth_center, normal, sun_direction, sky_irradiance);
    sphere_radiance =
        kSphereAlbedo * (1.0 / PI) * (sun_irradiance + sky_irradiance);
*/
    // Compute the radiance reflected by the ground.
  if (distance_to_intersection > 0.0) {
    vec3 point = camera + view_direction * distance_to_intersection;
    vec3 normal = normalize(point - earth_center);

    // Compute the radiance reflected by the ground.
    vec3 sky_irradiance;
    vec3 sun_irradiance = GetSunAndSkyIrradiance(
        point - earth_center, normal, sun_direction, sky_irradiance);
    ground_radiance = kGroundAlbedo * (1.0 / PI) * (
        sun_irradiance * GetSunVisibility(point, sun_direction) +
        sky_irradiance * GetSkyVisibility(point));

    float shadow_length = 0;
    vec3 transmittance;
    vec3 in_scatter = GetSkyRadianceToPoint(camera - earth_center,
        point - earth_center, shadow_length, sun_direction, transmittance);
    ground_radiance = ground_radiance * transmittance + in_scatter;
    ground_alpha = 1.0;
  }

/*
<p>Finally, we compute the radiance and transmittance of the sky, and composite
together, from back to front, the radiance and opacities of all the objects of
the scene:
*/

  // Compute the radiance of the sky.
  float shadow_length = 0;
  vec3 transmittance;
  vec3 radiance = GetSkyRadiance(
      camera - earth_center, view_direction, shadow_length, sun_direction,
      transmittance);

  // If the view ray intersects the Sun, add the Sun radiance.
  float dot_vdir_sundir = dot(view_direction, sun_direction);

  float holo_factor = 0;
  float solar_max = 0;
  float solar_min = 0;
  if (dot_vdir_sundir > (sun_holo_size)) {
    //  sun_holo_size < dot_vdir_sundir < sun_size.y
    solar_max = dot_vdir_sundir - sun_holo_size;
    solar_min = dot_vdir_sundir - sun_size.y;
    holo_factor = (dot_vdir_sundir - sun_holo_size)/ (sun_size.y - sun_holo_size);

    if (solar_min > 0){
      radiance = radiance + transmittance * GetSolarRadiance();
    } else {
      radiance = radiance + transmittance * GetSolarRadiance() *  exp(-50*(1-holo_factor)) * 0.0005;
      //radiance = radiance + transmittance * GetSolarRadiance() * clamp(-(2/3.1415) * atan(1000*(1-holo_factor)) + 1, 0, 1);
    }
  }
  radiance = mix(radiance, ground_radiance, ground_alpha);

/*
  ray_t ray = ray_t(camera, view_direction);
  float alpha;
  float cld = render_clouds(ray, sun_t(vec3(1, 1, 1), sun_direction), alpha);
  vec3 sky_irradiance;
  vec3 sun_irradiance = GetSunAndSkyIrradiance(
       -earth_center, sun_direction, sun_direction, sky_irradiance);

  radiance = mix(radiance, ((sun_irradiance + sky_irradiance) * cld), alpha);
  */

  //vec3 cldCol = sun_irradiance;

  //radiance = mix(radiance, shere_radiance, sphere_alpha);
  FragColor.rgb = //view_direction;
      vec3(1.0) - exp(-radiance / white_point * exposure), vec3(1.0 / 2.2);
      //normalize(GetSolarRadiance()) * holo_factor;
      //transmittance;
  
  //float cld = render_clouds(ray, sun_t(vec3(1, 1, 1), sun_direction));
  //FragColor.rgb = pow(vec3(1.0) - exp(-sky_irradiance/ white_point * exposure), vec3(1.0 / 1.0));

  //FragColor.rgb = cld.rgb;
  vec3 sky_irradiance;
    vec3 sun_irradiance = GetSunAndSkyIrradiance(
       -earth_center, sun_direction, sun_direction, sky_irradiance);
       
  FragColor.rgb = GetClouds(view_direction, sun_direction, FragColor.rgb, sun_irradiance, sky_irradiance);

//  FragColor.rgb = GetSolarLuminance();
  FragColor.a = 1.0;
}
