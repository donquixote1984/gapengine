#pragma once

#include "BasicMaterial.h"
#include <unordered_map>
#include "../textures/Texture.h"

class PBRMaterial: public BasicMaterial
{
protected:
   std::unordered_map<TextureType, Texture*> m_Textures; 
public:
    PBRMaterial();
};