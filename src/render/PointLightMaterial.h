#pragma once
#include "LightMaterial.h"
class PointLightMaterial: public LightMaterial
{
public:
    PointLightMaterial();
    void InitAttribs() override;
};