#shader vertex
#version 410
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 texCoord;

out vec2 v_TexCoord;
out vec4 FragPos;
void main()
{
    gl_Position = vec4(aPos.x, aPos.y, 0.0, 1.0);
    v_TexCoord = texCoord;
    FragPos = gl_Position;
}
	
#shader fragment
#version 410

out vec4 FragColor;
in vec2 v_TexCoord;
in vec4 FragPos;

uniform sampler2DArray u_CascadeShadowMap;
uniform float near_plane;
uniform float far_plane;
uniform int layer;

// required when using a perspective projection matrix
float LinearizeDepth(float depth)
{
    float z = depth * 2.0 - 1.0; // Back to NDC 
    return (2.0 * near_plane * far_plane) / (far_plane + near_plane - z * (far_plane - near_plane));	
}

void main()
{             
    float depthValue = texture(u_CascadeShadowMap, vec3(v_TexCoord, layer)).r;
   // FragColor = vec4(vec3(LinearizeDepth(depthValue) / far_plane), 1.0); // perspective
    FragColor = vec4(vec3(depthValue), 1.0); // orthographic
}