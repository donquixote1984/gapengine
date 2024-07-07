#include "DefaultMaterial.h"
#include "../shaders/GeometryShader.h"
DefaultMaterial::DefaultMaterial(): BasicMaterial()
{
    m_IntrinsicShader = GeometryShaderFactory::CreateGeometryShader(ShaderType::DEFAULT_SHADER);
    InitDefaultSnippets();
};

void DefaultMaterial::FeedTexturePackage(TexturePackagePath p)
{
    UpdateTextureFromPath(TextureType::AO_TEXTURE, p.ao);
    UpdateTextureFromPath(TextureType::DIFFUSE_TEXTURE, p.diffuse);
    UpdateTextureFromPath(TextureType::METALNESS_TEXTURE, p.metalness);
    UpdateTextureFromPath(TextureType::NORMAL_TEXTURE, p.normal);
    UpdateTextureFromPath(TextureType::OPACITY_TEXTURE, p.opacity);
    UpdateTextureFromPath(TextureType::ROUGHNESS_TEXTURE, p.roughness);
    UpdateTextureFromPath(TextureType::SPECULAR_TEXTURE, p.specular);
}

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