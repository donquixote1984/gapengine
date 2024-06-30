#include "NoiseRenderPass.h"
NoiseRenderPass::NoiseRenderPass()
{
    m_NoiseFrameBuffer = new NoiseFrameBuffer(NoiseType::PERLIN, Global::screen.width);
}

NoiseRenderPass::~NoiseRenderPass()
{
    delete m_NoiseFrameBuffer;
    m_NoiseFrameBuffer = nullptr;
}



void NoiseRenderPass::Render(RenderContext &rc) 
{
    m_NoiseFrameBuffer->IncOffsetX(m_Buffer[0]);
    m_NoiseFrameBuffer->IncOffsetY(m_Buffer[1]);
    m_NoiseFrameBuffer->SetFrequency((int)m_Buffer[2]);
    m_NoiseFrameBuffer->Render();
    m_NoiseFrameBuffer->ActiveTexture(constants::HEIGHT_MAP);
}