#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include "LightMap.h"

class PointLightMap: public LightMap
{
protected:
    void InitShaders() override;

public:
    PointLightMap(float width, float height);
    void OnRenderMVP(Geometry * geo, Light* light) override;
    void RenderToGeometry(Geometry *g, Light *l) override;
    void RenderDebug(Geometry *g, Light *) override;
    void OnRenderLightSpace(Light *) override;
    void Bind() override;
    
};