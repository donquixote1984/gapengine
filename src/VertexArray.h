#pragma once

#ifndef __VERTEX_BUFFER
#include "VertexBuffer.h"
#endif
#include "VertexBufferLayout.h"

class VertexArray
{
private:
    unsigned int m_RendererID; 
    int m_CurrentLayoutIndex = 0;
public:
    VertexArray();
    ~VertexArray();

    void AddBuffer(const VertexBuffer &vb, const VertexBufferLayout &layout, int divisor = 0);
    void Bind() const;
    void UnBind() const;
};