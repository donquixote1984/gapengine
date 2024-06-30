#include "RenderPass.h"
RenderPass::RenderPass()
{
    std::fill(m_Buffer,m_Buffer + RENDER_PASS_BUFFER_SIZE, 0);
}

void RenderPass::Render(RenderContext &rc) 
{}

RenderPassType RenderPass::GetType()
{
    return m_Type;
}
RenderPass::~RenderPass()
{
    if (m_Next != nullptr)
    {
        delete m_Next;
    }
}

void RenderPass::AppendNext(RenderPass *renderPass)
{
    this->m_Next = renderPass;
}

RenderPass * RenderPass::GetNext()
{
    return m_Next;
}
float * RenderPass::GetBuffer()
{
    return m_Buffer;
}

void RenderPass::Buffer(float data, int index)
{
    m_Buffer[index] = data;
}

FrameBuffer * RenderPass::GetFrameBuffer()
{
    return nullptr;
}