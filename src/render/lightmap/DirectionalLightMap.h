#pragma once
#include <glad/glad.h>
#include "LightMap.h"
#include "../../geometry/Geometry.h"
#include "../../lights/Light.h"

class DirectionalLightMap: public LightMap
{
protected:
    void InitShaders() override;
public:
    DirectionalLightMap(float width, float height);
    void OnRenderMVP(Geometry * g , Light * l) override;
    void OnRenderLightSpace(Light *) override;
    void RenderToGeometry(Geometry *g, Light *l) override;
    void RenderDebug(Geometry *g, Light *) override;
    void Bind() override;
    glm::mat4 GetLightSpaceMat(Light * l);
};