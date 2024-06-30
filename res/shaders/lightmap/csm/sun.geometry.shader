
#shader geometry
#version 410
layout(triangles) in;
layout(triangle_strip, max_vertices = 15) out;

uniform int u_UseCSM;

layout (std140) uniform SUN_CSM
{
    mat4 lightSpaceMatrices[5];
};

void main()
{
    for (int csm = 0; csm < 5; csm ++){
        gl_Layer = csm;
        for (int i = 0; i < 3 ; i++)
        {
            gl_Position = lightSpaceMatrices[csm] * gl_in[i].gl_Position;
            EmitVertex();
        }
        EndPrimitive();
    }
}
