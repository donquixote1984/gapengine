#pragma once
#include "Light.h"

class DirectionalLight: public Light 
{
private:

public:
    DirectionalLight();
    LightType GetLightType() const override;
    void GetImpact(LightImpact * impacts, int & size) override;
    void InitMaterial() override;
    float GetFarPlane() override;
    std::string GetName() override;
    ShaderDirLight Serialize();
};