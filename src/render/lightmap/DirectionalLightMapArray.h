#pragma once
#include "LightMapArray.h"
#include "../../global/ScreenProps.h"
#include "../../Config.h"
#include "LightMapUtils.h"
class DirectionalLightMapArray: public LightMapArray
{
private:
    float lvls[constants::MAX_CSM_LVLS - 1] = {.02f, .08f, .3f, .5f};
    std::vector<float> m_ShadowCascadeLevels;
    bool m_UseCSM;
    Camera * m_Camera;
    
    void InitShaders() override;
protected:
    void CalculateLightSpaceMatricesCSM();
    void CalculateLightSpaceMatrices();
    void OnRenderGeometryCSM(Geometry * g);
public:
    DirectionalLightMapArray();
    ~DirectionalLightMapArray();
    void Bind() override;
    void ActiveTexture() override;
    void OnRenderLightSpace() override;
    void RenderDebug(Geometry*g) override;
    void EnableCSM(bool csm);
    void OnRenderGeometry(Geometry * g, RenderContext &rc) override;
    bool HasLights() override;
};