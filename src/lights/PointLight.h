#pragma once
#include "Light.h"
#include "../render/lightmap/PointLightMap.h"
#include "../render/lightmap/DirectionalLightMap.h"
#include "../global/ScreenProps.h"

class PointLight: public Light
{

public:
    std::vector<glm::mat4> shadowTranforms;
    PointLight();
    ~PointLight();
    LightType GetLightType() const override;
    void GetImpact(LightImpact *impacts, int &size) override;
    void InitMaterial() override;
    float GetFarPlane() override;
    std::string GetName() override;
    ShaderPointLight Serialize();

};
