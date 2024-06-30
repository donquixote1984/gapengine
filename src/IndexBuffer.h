#pragma once

class IndexBuffer
{
private:
    unsigned int m_RendererID;
    unsigned int m_Count;
public:
    IndexBuffer();
    ~IndexBuffer();

    void Bind() const;
    void Unbind() const;
    void Feed(const void * data, unsigned int count);
    inline unsigned int GetCount() const { return m_Count; }
};