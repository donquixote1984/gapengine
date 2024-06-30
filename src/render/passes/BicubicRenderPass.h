#pragma once
#include "RenderPass.h"
#include "../../Bufferable.h"
#include "../../ScreenCanvas.h"
#include "../../shaders/Shader.h"

class BicubicRenderPass: public RenderPass
{
private:
    Bufferable *m_Bufferable = nullptr;
    Shader *m_Shader;
    ScreenCanvas m_ScreenCanvas;
public:
    BicubicRenderPass(Bufferable *b);
    ~BicubicRenderPass();
    void Render(RenderContext &rc) override;
};