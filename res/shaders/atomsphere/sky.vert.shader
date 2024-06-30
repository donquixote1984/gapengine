#shader vertex
#version 410
layout (location = 0) in vec2 aPos;


layout (std140) uniform Matrices
{
    mat4 projection;
    mat4 view;
};


out vec3 view_ray;
void main() {
    gl_Position = vec4(aPos.x, aPos.y, 0, 1.0).xyww;
    view_ray = (inverse(view) * vec4((inverse(projection) * vec4(aPos.x, aPos.y, 0, 1.0)).xyz, 0.0)).xyz;
    //view_ray = cross(cross(view_ray, vec3(0,1,0)), view_ray);
}