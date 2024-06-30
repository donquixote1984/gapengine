uniform int u_UseCSM;
uniform int cascadeCount;
uniform float u_CascadePlaneDistances[5];

layout (std140) uniform CSM
{
    mat4 lightSpaceMatrices[50];
};
layout (std140) uniform SUN_CSM
{
    mat4 lightSpaceMatricesSun[5];
};

int GetShadowCSMLayer()
{
    if (u_UseCSM == 0)
    {
        return 0;
    }
    float depthValue = abs(FragViewPos.z);
    int layer = -1;
    for (int i = 0; i < 5; i++)
    {
        if (depthValue <= u_CascadePlaneDistances[i])
        {
            layer = i;
            break;
        }
    }

    if (layer == -1)
    {
        layer = 4;
    }
    return layer;
}

int GetShadowSunCSMLayer()
{
    float depthValue = abs(FragViewPos.z);
    int layer = -1;
    for (int i = 0; i < 5; i++)
    {
        if (depthValue <= u_CascadePlaneDistances[i])
        {
            layer = i;
            break;
        }
    }

    if (layer == -1)
    {
        layer = 4;
    }
    return layer;
}