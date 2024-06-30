#pragma once
#include <glad/glad.h>
#include <memory>
#include "LightMapArray.h"
#include "../../lights/Light.h"
#include "../../Camera.h"
#include <vector>
#include "../../global/ScreenProps.h"
#include "../../UniformBuffer.h"
#include "../../ui/UISettings.h"
#include "LightMapUtils.h"
#include "../../lights/SunLight.h"

class SunLightMapCSM: public LightMapArray
{
protected:
    Camera *m_Camera = nullptr;
    float lvls[constants::MAX_CSM_LVLS - 1] = {.2f, .3f, .4f, .5f};
    std::vector<float> m_ShadowCascadeLevels;
    SunLight * m_SunLight = nullptr;
    
    void InitShaders() override;
    void OnRenderGeometryCSM(Geometry *g);
    
public:
    SunLightMapCSM();
    ~SunLightMapCSM();
    //void OnRenderMVP(Geometry * g , Light * l) override;
    void ActiveTexture() override;
    void OnRenderLightSpace() override;
    void OnRenderGeometry(Geometry * g) override;
    void RenderDebug(Geometry *g) override;
    void Bind() override;
    void CalculateLightSpaceMatricesCSM();
    bool HasLights() override;
    void SetSunLight(SunLight * light);
};