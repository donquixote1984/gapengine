#include "DefaultMaterial.h"
#include "../shaders/GeometryShader.h"
DefaultMaterial::DefaultMaterial(): BasicMaterial()
{
    m_IntrinsicShader = GeometryShaderFactory::CreateGeometryShader(ShaderType::DEFAULT_SHADER);
    InitDefaultSnippets();
};

void DefaultMaterial::FeedMeshMaterial(MeshMaterial mm)
{
    if (!mm.valid)
    {
        return;
    }

    for (MeshTexture t : mm.textures)
    {
        if (!t.valid)
        {
            continue;
        }
        UpdateTexture(t);
    }
}

void DefaultMaterial::UpdateTexture(MeshTexture t)
{
    if (t.embeded)
    {
        UpdateTextureFromMemory(t.type, t.data);
    }
    else
    {
        UpdateTextureFromPath(t.type, t.path);
    }
}