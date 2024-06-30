#pragma once
#include "Material.h"
#include "MaterialAttrib.h"
#include <unordered_map>

class CustomMaterial: public Material
{
private:
    std::unordered_map<std::string, MaterialAttrib> kv;
public:
    CustomMaterial();
    CustomMaterial(const std::string shaderfile);
    ~CustomMaterial();
    void FlushToShader() override;
    void InitAttribs() override;
    void SetValue(std::string key, MaterialAttrib ma);
};