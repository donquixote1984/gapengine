#include "FrameCacheRenderPass.h"
#include "../../global/ScreenProps.h"
#include "../../Config.h"
FrameCacheRenderPass::FrameCacheRenderPass(Bufferable *b)
{
    m_Bufferable = b;
    m_FrameBuffer = new FrameBuffer(Global::screen.width, Global::screen.height);
    m_Shader = new Shader("res/shaders/postprocessing/basic.shader");
    m_FrameBuffer->AddDrawShader(m_Shader);
}
FrameCacheRenderPass::~FrameCacheRenderPass()
{
    delete m_FrameBuffer;
    delete m_Shader;
}

void FrameCacheRenderPass::Render(RenderContext &rc) // render prev frame into it's own texture.
{
    m_Shader->Bind();
    m_Bufferable->ActiveTexture(0);
    m_Shader->setUniform1i("screenTexture", 0);
    m_FrameBuffer->Render();
    m_FrameBuffer->ActiveTexture(constants::FRAME_CACHE);
}
FrameBuffer * FrameCacheRenderPass::GetFrameBuffer()
{
    return m_FrameBuffer;
}