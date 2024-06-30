#pragma once
#include "RenderPass.h"
#include "../../FrameBuffer.h"
#include "../../Bufferable.h"
#include "../../shaders/Shader.h"
class FrameCacheRenderPass: public RenderPass
{
private:
    FrameBuffer *m_FrameBuffer;
    Bufferable *m_Bufferable;
    Shader *m_Shader;
public:
    FrameCacheRenderPass(Bufferable *b);
    ~FrameCacheRenderPass();
    void Render(RenderContext &rc) override;
    FrameBuffer * GetFrameBuffer() override;
};