#pragma once
#include "RenderPass.h"
#include "../../FrameBuffer.h"
#include "../../shaders/Shader.h"
class CloudNoiseLutRenderPass: public RenderPass
{
private:
    FrameBuffer *m_FrameBuffer;
    Shader * m_Shader;
    bool m_FirstRender = true;
public:
    CloudNoiseLutRenderPass();
    ~CloudNoiseLutRenderPass();
    void Render(RenderContext &rc) override;
};