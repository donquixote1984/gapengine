#include "Bufferable.h"

Bufferable::Bufferable(float width, float height)
{
    m_FrameBuffer = new FrameBuffer(width, height);
}
Bufferable::~Bufferable()
{
    if (m_FrameBuffer != nullptr) 
    {
        delete m_FrameBuffer;
        m_FrameBuffer = nullptr;
    }
}

void Bufferable::Bind()
{
    m_FrameBuffer->Bind();
}

void Bufferable::Bind(float width, float height)
{
    m_FrameBuffer->Bind(width, height);
}

void Bufferable::ActiveTexture(int slot) 
{
    m_FrameBuffer->ActiveTexture(slot);
}

FrameBuffer * Bufferable::GetFrameBuffer()
{
    return m_FrameBuffer;
}