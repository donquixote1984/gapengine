#pragma once

#include "Material.h"
#include <glm/glm.hpp>
#include "MaterialAttrib.h"
#include "../shaders/ShaderSnippet.h"
#include "../ui/UISettings.h"
#include "../textures/MemoryTextureData.h"

class BasicMaterial: public Material
{
private:
public:
    MaterialAttrib ambient;
    MaterialAttrib diffuse;
    MaterialAttrib specular;
    MaterialAttrib roughness;
    MaterialAttrib metalness;
    MaterialAttrib normal;

    MaterialAttrib shininess;
    MaterialAttrib tile;

    bool tilable = false;
    BasicMaterial();
    ~BasicMaterial();
    void FlushToShader() override;
    void FlushToShader(ShaderUniformsCache& cache);
    
    void BindOutsideShaderAttrib(MaterialAttrib mAttrib) const;
    void InitAttribs() override;
    void SetTilable(bool tileable);
    void SetTangent(bool hasNormal);
    void SetShadow(bool receiveShadow);
    void InitDefaultSnippets();
    void UpdateTextureFromMemory(TextureType type, MemoryTextureData data);
    void UpdateTextureFromPath(TextureType type, std::string path);
};