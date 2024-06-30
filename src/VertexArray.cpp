#include "VertexArray.h"

VertexArray::VertexArray()
{
    glGenVertexArrays(1, &m_RendererID);
}

VertexArray::~VertexArray()
{
    glDeleteVertexArrays(1, &m_RendererID);
}
void VertexArray::AddBuffer(const VertexBuffer & vb,  const VertexBufferLayout & layout, int divisor)
{
    Bind();
    vb.Bind();
    unsigned int offset = 0;
    const auto& elements = layout.GetElements();
    for(unsigned int i = 0; i < elements.size() ; i ++)
    {
        int layoutIndex = i + m_CurrentLayoutIndex;
        const auto& element = elements[i];
        glEnableVertexAttribArray(layoutIndex);
        //glVertexAttribPointer()
        glVertexAttribPointer(layoutIndex, element.count, element.type, element.normalized, layout.GetStride(), (void *)offset);
        offset += element.count * VertexBufferElement::GetSizeOfType(element.type);
        if (divisor > 0)
        {
            glVertexAttribDivisor(layoutIndex, divisor);
        }
    }
    m_CurrentLayoutIndex += elements.size();
    UnBind();
    vb.Unbind();
}
void VertexArray::Bind() const
{
    glBindVertexArray(m_RendererID);
}

void VertexArray::UnBind() const
{
    glBindVertexArray(0);
}