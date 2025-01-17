#shader vertex
#version 410
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTex;

out vec2 tex;

uniform mat4 model;

layout (std140) uniform Matrices
{
    mat4 projection;
    mat4 view;
};


void main()
{
    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
    tex = aTex;
}

#include "../terrain/tessellation.shader"
#include "common.pbr.fragment.shader"