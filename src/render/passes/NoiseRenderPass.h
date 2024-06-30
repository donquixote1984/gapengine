#pragma once
#include "RenderPass.h"
#include "../../alg/NoiseFrameBuffer.h"
class NoiseRenderPass: public RenderPass
{
private:
    
    NoiseFrameBuffer *m_NoiseFrameBuffer;
public:
    NoiseRenderPass();
    ~NoiseRenderPass();
    void Render(RenderContext &rc) override;
};