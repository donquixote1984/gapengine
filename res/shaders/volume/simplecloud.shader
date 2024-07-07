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

#define MAX_POINT_LIGHTS 10
#define MAX_DIR_LIGHTS 10

#include "../partials/fragment/light.struct.h.shader"
// lights
layout (std140) uniform Lights 
{
    int ub_StaticPointLightNum;
    int ub_DynamicPointLightNum;
    int ub_StaticDirLightNum;
    int ub_DynamicDirLightNum;

    PointLight ub_StaticPointLights[MAX_POINT_LIGHTS];
    PointLight ub_DynamicPointLights[MAX_POINT_LIGHTS];
    
    DirLight ub_StaticDirLights[MAX_DIR_LIGHTS];
    DirLight ub_DynamicDirLights[MAX_DIR_LIGHTS];

    SunLight ub_SunLight;
};

uniform float u_Time;
uniform vec2 u_Resolution;
uniform sampler2D u_NoiseTexture;
uniform sampler2D u_DitherTexture;
uniform int u_Frame;

#define MAX_STEPS 50
const float MARCH_SIZE = 0.16;

float sdSphere(vec3 p, float radius) {
  return length(p) - radius;
}

float sdBox( vec3 p, vec3 b )
{
  vec3 q = abs(p) - b;
  return length(max(q,0.0)) + min(max(q.x,max(q.y,q.z)),0.0);
}

float noise(vec3 x ) {
  vec3 p = floor(x);
  vec3 f = fract(x);
  f = f*f*(3.0-2.0*f);

  vec2 uv = (p.xy+vec2(37.0,239.0)*p.z) + f.xy;
  vec2 tex = textureLod(u_NoiseTexture, (uv+0.5)/256.0,0.0).yx;

  return mix( tex.x, tex.y, f.z ) * 2.0 - 1.0;
}

float fbm(vec3 p) {
  vec3 q = p + u_Time* 0.5 * vec3(1.0, -0.2, -1.0);
  float g = noise(q);

  float f = 0.0;
  float scale = 0.5;
  float factor = 2.02;

  for (int i = 0; i < 6; i++) {
      f += scale * noise(q);
      q *= factor;
      factor += 0.21;
      scale *= 0.5;
  }

  return f;
}

float scene(vec3 p) {
  float distance = sdBox(p, vec3(2, 0.1, 2));
  float f = fbm(p);
  return -distance + f;
}

vec4 raymarch(vec3 rayOrigin, vec3 rayDirection, float offset) {
  float depth = 0.0;
  depth += MARCH_SIZE * offset;
  vec3 p = rayOrigin + depth * rayDirection;
  
  vec4 res = vec4(0.0);

  for (int i = 0; i < MAX_STEPS; i++) {
    float density = scene(p);
    // We only draw the density if it's greater than 0
    if (density > 0.0) {

      float diffuse = clamp((scene(p) - scene(p + 0.3 * ub_DynamicDirLights[0].direction)) / 0.3, 0.0, 1.0 );
      vec3 lin = vec3(0.60,0.60,0.75) * 1.1 + 0.8 * vec3(1.0,0.6,0.3) * diffuse;

      vec4 color = vec4(mix(vec3(1.0,1.0,1.0), vec3(0.0, 0.0, 0.0), density), density );
      color.rgb *= lin;
      
      color.rgb *= color.a;
      res += color*(1.0-res.a);
    }

    depth += MARCH_SIZE;
    p = rayOrigin + depth * rayDirection;
  }

  return res;
}

out vec4 FragColor;
void main() {
  vec2 uv = gl_FragCoord.xy/u_Resolution.xy;
  uv -= 0.5;
  uv.x *= u_Resolution.x / u_Resolution.y;

  // Ray Origin - camera
  vec3 ro = vec3(0.0, 0.0, 5.0);
  // Ray Direction
  vec3 rd = normalize(vec3(uv, -1.0));
  
  vec3 color = vec3(0.0);
  float blueNoise = texture(u_DitherTexture, gl_FragCoord.xy / 1024.0).r;
  float offset = fract(blueNoise + float(u_Frame%32) / sqrt(0.5));

  vec4 res = raymarch(ro, rd, offset);
  color = res.rgb;

  FragColor = vec4(color, 1.0);
}
