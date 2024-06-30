#pragma once
#include "FrameBuffer.h"
#include "ScreenCanvas.h"
#include "render/passes/SceneRenderPass.h"
class FrameBufferDebugger
{
private:
    FrameBuffer *m_CurrentFrameBuffer = nullptr;
    ScreenCanvas *m_ScreenCanvas = nullptr;

public:
    FrameBufferDebugger();
    ~FrameBufferDebugger();
    static FrameBufferDebugger * get();
    void Debug();
    void SetCurrentFrameBuffer(FrameBuffer *fb);
};

class FrameBufferDebugRenderPass: public SceneRenderPass
{
private:
    FrameBuffer * m_CurrentFrameBuffer = nullptr;
    ScreenCanvas *m_ScreenCanvas = nullptr;
public:
    FrameBufferDebugRenderPass();
    ~FrameBufferDebugRenderPass();
    void Render(RenderContext &rc) override;
    void SetCurrentFrameBuffer(FrameBuffer *fb);
};