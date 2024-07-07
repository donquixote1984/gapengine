#pragma once

#include "BasicMaterial.h"
#include <unordered_map>
#include "../textures/Texture.h"
#include "../Mesh.h"
#include "../reader/TextureReader.h"

class DefaultMaterial: public BasicMaterial
{
protected:
   std::unordered_map<TextureType, Texture*> m_Textures; 
public:
    DefaultMaterial();
    void FeedMeshMaterial(MeshMaterial mm);
    void UpdateTexture(MeshTexture t);
    void FeedTexturePackage(TexturePackagePath p);
};