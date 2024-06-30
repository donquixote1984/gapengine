#pragma once
#include <array>
#include "RenderPassType.h"
#include "../RenderContext.h"
#include "../../FrameBuffer.h"

#define RENDER_PASS_BUFFER_SIZE  16

class RenderPass
{
protected:
    RenderPassType m_Type;
    RenderPass *m_Next = nullptr;
    float m_Buffer[RENDER_PASS_BUFFER_SIZE];
public:
    RenderPass();
    virtual void Render(RenderContext &rc) = 0;
    virtual ~RenderPass();
    virtual RenderPassType GetType();
    void AppendNext(RenderPass *);
    RenderPass * GetNext();
    float * GetBuffer();
    void Buffer(float data, int index);
    virtual FrameBuffer *GetFrameBuffer();
};