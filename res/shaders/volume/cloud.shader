#include "vertex.include.shader"
#include "common.shader"
/**
https://www.shadertoy.com/view/XtBXDw
*/


//-------------------------------------------------------------------------------------

out vec4 FragColor;

void main()
{

	vec3 col = vec3(0, 0, 0);

	//mat3 rot = rotate_around_x(abs(sin(u_time / 2.)) * 45.);
	//sun_dir = mul(rot, sun_dir);

	vec3 eye = vec3(0, 1., 10);
	vec3 look_at = vec3(0, 1.6, -1);
	//ray_t eye_ray = get_primary_ray(point_cam, eye, look_at);

	//eye_ray.direction.yz = mul(rotate_2d(+u_mouse.y * .13), eye_ray.direction.yz);
	//eye_ray.direction.xz = mul(rotate_2d(-u_mouse.x * .33), eye_ray.direction.xz);
	ray_t eye_ray;
    eye_ray.origin = eye;
    eye_ray.direction = normalize(view_ray);

	//intersect_plane(eye_ray, ground, hit);

	vec4 cld = vec4(0);
	if (!intersect_plane(eye_ray, ground))
	{
    //vec3 sky = render_sky_color(eye_ray);
    	cld = render_clouds(eye_ray);
	}
	
	FragColor = cld;
}