#pragma once

#include "BasicMaterial.h"
#include <unordered_map>
#include "../textures/Texture.h"

class TerrainMaterial: public BasicMaterial
{
protected:
   std::unordered_map<TextureType, Texture*> m_Textures; 
public:
    TerrainMaterial();
    ~TerrainMaterial();
};