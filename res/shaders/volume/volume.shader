#shader vertex
#version 410

layout (location = 0) in vec3 aPos;

layout (std140) uniform Matrices
{
    mat4 projection;
    mat4 view;
};

void main()
{
    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
}
	

#shader fragment
#version 410
#define MAX_STEPS  80
#define STEP_SIZE  0.1
#define MAX_DIST  100
#define EPS  0.01
out vec4 FragColor;

uniform vec2 u_Resolution;
uniform float u_Time;

mat4 rotation3d(vec3 axis, float angle) {
  axis = normalize(axis);
  float s = sin(angle);
  float c = cos(angle);
  float oc = 1.0 - c;

  return mat4(
    oc * axis.x * axis.x + c,           oc * axis.x * axis.y - axis.z * s,  oc * axis.z * axis.x + axis.y * s,  0.0,
    oc * axis.x * axis.y + axis.z * s,  oc * axis.y * axis.y + c,           oc * axis.y * axis.z - axis.x * s,  0.0,
    oc * axis.z * axis.x - axis.y * s,  oc * axis.y * axis.z + axis.x * s,  oc * axis.z * axis.z + c,           0.0,
    0.0,                                0.0,                                0.0,                                1.0
  );
}

vec3 rotate(vec3 v, vec3 axis, float angle) {
  mat4 m = rotation3d(axis, angle);
  return (m * vec4(v, 1.0)).xyz;
}

float smoothmin(float a, float b, float k) {
  float h = clamp(0.5 + 0.5 * (b-a)/k, 0.0, 1.0);
  return mix(b, a, h) - k * h * (1.0 - h);
}
float sdSphere(vec3 p, float radius)
{
  return length(p) - radius;
}

float sdBox( vec3 p, vec3 b )
{
  vec3 q = abs(p) - b;
  return length(max(q,0.0)) + min(max(q.x,max(q.y,q.z)),0.0);
}

vec3 repeat(vec3 p, float c)
{
  return mod(p,c) - 0.5 * c;
}
float scene(vec3 p)
{
  vec3 p1 = repeat(p , 4);
  float distance1 = sdSphere(p1, 0.5);
  return distance1;
}


vec3 getNormal(vec3 p) {
  vec2 e = vec2(.01, 0);

  vec3 n = scene(p) - vec3(
    scene(p-e.xyy),
    scene(p-e.yxy),
    scene(p-e.yyx));

  return normalize(n);
}



float raymarch(vec3 rayo, vec3 rayd)
{
  //vec3 d0 = vec3(0);
  float d0 = 0;
  for (int i = 0; i < MAX_STEPS; i++)
  {
    vec3 p = rayo + rayd * d0;
    float ds = scene(p);
    d0 += ds;
    if (ds < EPS || d0 > MAX_DIST)
    {
      return d0;
    }
  }

  return d0;
}

void main()
{
  vec2 uv = gl_FragCoord.xy / u_Resolution.xy;
  uv -=0.5;
  uv.x *= u_Resolution.x / u_Resolution.y;
  vec3 rayOrigin = vec3(0, 0, 5);
  vec3 rayDir = normalize(vec3(uv, -1));
  vec3 lightPosition = vec3(-10.0 * cos(u_Time), 10.0, 10.0 * sin(u_Time));

  float d = raymarch(rayOrigin, rayDir);
  vec3 p = rayOrigin + rayDir * d;
  if (d >= MAX_DIST)
  {
    FragColor = vec4(vec3(0), 1);
  } else {

     vec3 lightDirection = normalize(lightPosition - p);
    vec3 normal = getNormal(p);
    float diffuse = max(dot(lightDirection, normal), 0);
    FragColor = vec4(vec3(diffuse), 1);
  }
}