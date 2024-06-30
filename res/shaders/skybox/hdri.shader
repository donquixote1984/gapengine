#shader vertex
#version 410
layout (location = 0) in vec3 aPos;

out vec3 v_TexCoord;

uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position= projection * view * vec4(aPos.x, aPos.y, aPos.z, 1.0);
    v_TexCoord = aPos;
}
	
#shader fragment
#version 410
in vec3 v_TexCoord;
out vec4 FragColor;

uniform sampler2D hdriMap;
const vec2 invAtan = vec2(0.1591, 0.3183);

vec2 SampleSphericalMap(vec3 v)
{
    vec2 uv = vec2(atan(v.z, v.x), asin(v.y));
    uv *= invAtan;
    uv += 0.5;
    return uv;
}

void main()
{
    vec2 uv = SampleSphericalMap(normalize(v_TexCoord));
    vec3 color = texture(hdriMap, uv).rgb;
    FragColor = vec4(color, 1.0);
}