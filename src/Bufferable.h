#pragma once
#include "FrameBuffer.h"
#include "global/ScreenProps.h"

class Bufferable
{
protected:
    FrameBuffer *m_FrameBuffer = nullptr;
public:
    Bufferable(float width = Global::screen.width, float height = Global::screen.height);
    ~Bufferable();
    void ActiveTexture(int slot);
    void Bind();
    void Bind(float width, float height);
    FrameBuffer * GetFrameBuffer();
};