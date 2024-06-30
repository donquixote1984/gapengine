
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
out vec3 view_ray;
void main()
{
    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
    view_ray = (inverse(view) * vec4((inverse(projection) * vec4(aPos.x, aPos.y, 0, 1.0)).xyz, 0.0)).xyz;
    TexCoords = aTexCoords;
}
	

#shader fragment
#version 410

in vec2 TexCoords;
in vec3 view_ray;