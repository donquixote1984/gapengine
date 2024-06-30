#pragma once

#include <iostream>
#include "geometry/RawVertices.h"
#include "shaders/Shader.h"
#include "util/CommonData.h"
#include "exceptions/FrameBufferInitializeException.h"
#include "global/ScreenProps.h"

class FrameBuffer: public RawVertices
{
protected:
    unsigned int m_FrameBuffer;
    unsigned int m_TextureId;
    unsigned int m_RBO;

    float m_BufferWidth;
    float m_BufferHeight;
    
    Shader *m_Shader = nullptr;
    Shader *m_ScreenShader = nullptr;
public:
    FrameBuffer();
    FrameBuffer(float width, float height, bool wrap = false);
    FrameBuffer(Shader * shader, float width, float height);
    ~FrameBuffer();
    void ActiveTexture(int slot = 0);
    virtual void Render();
    void FlushToScreen();
    void FlushToWindow();
    void AddDrawShader(Shader * shader);
    void Bind() override;
    void Bind(float width, float height);
    void UnBind() override;
    Shader * GetShader();
};