#shader geometry
#version 410
layout(triangles) in;
layout(triangle_strip, max_vertices = 150) out;

uniform int u_DirLightNum;

uniform int u_UseCSM;

layout (std140) uniform CSM
{
    mat4 lightSpaceMatrices[50];
};

void main()
{
    int loopsize = 1;
    if (u_UseCSM == 1) {
        loopsize = 5;
    }

    for (int dnum = 0; dnum < u_DirLightNum; dnum++) 
    {
        for (int csm = 0; csm < loopsize; csm ++){
            gl_Layer = dnum * 5+ csm;
            for (int i = 0; i < 3 ; i++)
            {
                gl_Position = lightSpaceMatrices[dnum * 5 + csm] * gl_in[i].gl_Position;
                EmitVertex();
            }
            EndPrimitive();
        }
    }
}
