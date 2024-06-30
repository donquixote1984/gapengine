#pragma once
#include "DirectionalLight.h"
class SunLight : public DirectionalLight
{
public:
    ShaderSunLight Serialize();
    LightType GetLightType() const override;
};