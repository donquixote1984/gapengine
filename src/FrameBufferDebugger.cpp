#include "FrameBufferDebugger.h"
FrameBufferDebugger::FrameBufferDebugger()
{
    m_ScreenCanvas = new ScreenCanvas(false);
}
FrameBufferDebugger::~FrameBufferDebugger()
{
    delete m_ScreenCanvas;
}

FrameBufferDebugger * FrameBufferDebugger::get()
{
    static FrameBufferDebugger fbd;
    return &fbd;
}

void FrameBufferDebugger::Debug()
{
    
    m_CurrentFrameBuffer->FlushToWindow();
    m_ScreenCanvas->Draw();

}

void FrameBufferDebugger::SetCurrentFrameBuffer(FrameBuffer *fb)
{
    m_CurrentFrameBuffer = fb;
}

FrameBufferDebugRenderPass::FrameBufferDebugRenderPass()
{
    m_ScreenCanvas = new ScreenCanvas(true);
}


FrameBufferDebugRenderPass::~FrameBufferDebugRenderPass()
{
    delete m_ScreenCanvas;
}


void FrameBufferDebugRenderPass::Render(RenderContext &rc) 
{
    if (m_CurrentFrameBuffer != nullptr)
    {
        m_CurrentFrameBuffer->FlushToScreen();
        m_ScreenCanvas->Draw();
        glEnable(GL_DEPTH_TEST);
    }
}


void FrameBufferDebugRenderPass::SetCurrentFrameBuffer(FrameBuffer *fb) 
{
    m_CurrentFrameBuffer = fb;
}
