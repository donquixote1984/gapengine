#pragma once
#include "LightMaterial.h"
class DirectionalLightMaterial: public LightMaterial
{
public:
    DirectionalLightMaterial();
    void InitAttribs() override;
};