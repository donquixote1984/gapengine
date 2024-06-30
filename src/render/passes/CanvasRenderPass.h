#pragma once
#include "RenderPass.h"
#include "../../ScreenCanvas.h"
#include "../../shaders/Shader.h"

class ShadedEmptyTransform;
class CanvasRenderPass: public RenderPass
{
private:
    ScreenCanvas m_ScreenCanvas;
    ShadedEmptyTransform * m_Geo;
    void OnRenderMaterial(RenderContext &rc) const;
    
public:
    CanvasRenderPass(ShadedEmptyTransform *);
    void Render(RenderContext &rc) override;
};