#pragma once
#include "RenderPass.h"
#include "../../ui/UISettings.h"
#include "../../global/Sky.h"
#include "../../geometry/Geometry.h"
#include "../../UniformBuffer.h"

class GeometryRenderPass: public RenderPass
{
private:
    void OnRenderMVP(RenderContext &rc) const;
    void OnRenderMaterial(RenderContext &rc) const;
    Geometry * m_Geo;
public:
    GeometryRenderPass(Geometry *g);
    void Render(RenderContext &rc) override;
    
};