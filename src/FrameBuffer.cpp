#include "FrameBuffer.h"

FrameBuffer::FrameBuffer() {}
FrameBuffer::FrameBuffer(float width, float height, bool wrap):m_BufferWidth(width), m_BufferHeight(height), RawVertices(util::UnitQuad, 6, {2, 2})
{
    glGenFramebuffers(1, &m_FrameBuffer);
    glGenTextures(1, &m_TextureId);
    glGenRenderbuffers(1, &m_RBO);
    glBindTexture(GL_TEXTURE_2D, m_TextureId);

    //glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_BufferWidth, m_BufferHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_BufferWidth, m_BufferHeight, 0, GL_RGB, GL_FLOAT, 0);
    if (wrap) 
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    } else {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    }

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, m_RBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, m_BufferWidth, m_BufferHeight);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_RBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_TextureId, 0);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        throw FrameBufferInitializeException("FrameBuffer init error");
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    m_ScreenShader = new Shader("./res/shaders/postprocessing/basic.shader");

}
FrameBuffer::FrameBuffer(Shader * shader, float width, float height): m_Shader(shader), m_BufferWidth(width), m_BufferHeight(height)
{
    FrameBuffer(width, height);
}
void FrameBuffer::AddDrawShader(Shader * shader)
{
    m_Shader = shader;
}
void FrameBuffer::Bind()
{
    glViewport(0, 0, m_BufferWidth, m_BufferHeight);
    glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBuffer);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glBindTexture(GL_TEXTURE_2D, m_TextureId);
}
void FrameBuffer::Bind(float width, float height)
{
    glViewport(0, 0, width, height);
    glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBuffer);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //glBindTexture(GL_TEXTURE_2D, m_TextureId);
}
void FrameBuffer::UnBind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

FrameBuffer::~FrameBuffer()
{
    delete m_ScreenShader;
    m_ScreenShader = nullptr;
    glDeleteFramebuffers(1, &m_FrameBuffer);
    glDeleteRenderbuffers(1, &m_RBO);
    glDeleteTextures(1, &m_TextureId);
}

void FrameBuffer::ActiveTexture(int slot)
{
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, m_TextureId);
}

void FrameBuffer::Render()
{
    glViewport(0, 0, m_BufferWidth, m_BufferHeight);
    glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBuffer);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //glBindTexture(GL_TEXTURE_2D, m_TextureId);

    m_Shader->Bind();
    m_VAO.Bind();
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBuffer::FlushToScreen()
{
    glViewport(0, 0, m_BufferWidth, m_BufferWidth);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glDisable(GL_DEPTH_TEST);
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    //glBindVertexArray(m_VAO);
    m_VAO.Bind();
    m_ScreenShader->Bind();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_TextureId);
    m_ScreenShader->setUniform1i("screenTexture", 0);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glEnable(GL_DEPTH_TEST);
}

void FrameBuffer::FlushToWindow()
{
    glViewport(0, 0, m_BufferWidth, m_BufferHeight);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glDisable(GL_DEPTH_TEST);
    m_ScreenShader->Bind();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_TextureId);
    m_ScreenShader->setUniform1i("screenTexture", 0);
    glEnable(GL_DEPTH_TEST);
//    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}


Shader * FrameBuffer::GetShader()
{
    return m_Shader;
}