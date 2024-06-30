#pragma once
#include "VertexBuffer.h"
#include "VertexArray.h"
#include "VertexBufferLayout.h"

class ScreenCanvas
{
private:
    VertexBuffer m_VB;
    VertexArray m_VAO;
    VertexBufferLayout m_Layout;
    void InitVertices(bool fullScreen = true);
public:
    ScreenCanvas(bool fullScreen = true);
    void Draw() const;
};