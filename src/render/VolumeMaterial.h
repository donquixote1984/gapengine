#pragma once
#include "Material.h"
#include "MaterialAttrib.h"
class VolumeMaterial: public Material
{
public:
    MaterialAttrib tint;
    MaterialAttrib density;

    VolumeMaterial();
    ~VolumeMaterial();

    void FlushToShader() override;
    void InitAttribs() override;
};