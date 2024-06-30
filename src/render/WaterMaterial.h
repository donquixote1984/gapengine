#pragma once

#include "Material.h"
#include "MaterialAttrib.h"

class WaterMaterial: public Material
{
public:
    MaterialAttrib tint;
    MaterialAttrib transparency;
    MaterialAttrib flow;
    MaterialAttrib waveStrength;
    MaterialAttrib waveSpeed;
    MaterialAttrib normal;

    WaterMaterial();
    ~WaterMaterial();
    void FlushToShader() override;
    void InitAttribs() override;
};