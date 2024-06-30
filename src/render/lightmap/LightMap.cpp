#include "LightMap.h"

LightMap::LightMap(float width, float height): m_ScreenWidth(width), m_ScreenHeight(height)
{}

LightMap::~LightMap()
{
    delete m_LightMapShader;
    delete m_LightMapDebugShader;
}

void LightMap::EnableDebug()
{
    m_IsDebugging = true;
    m_LightMapDebugShader->Bind();
    m_LightMapDebugShader->UnBind();
}

bool LightMap::IsDebugging()
{
    return m_IsDebugging;
}
void LightMap::Flush()
{
    m_ScreenCanvas.Draw();
}

void LightMap::GetSize(float &width, float &height)
{
    width = m_ScreenWidth;
    height = m_ScreenHeight;
}

void LightMap::UnBind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, m_ScreenWidth, m_ScreenHeight);
    //glClearColor(0.1f, 0.1f, 0.5f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //glCullFace(GL_BACK);
}
float LightMap::GetFarPlane()
{
    return m_FarPlane;
}

void LightMap::SetIndex(unsigned int index)
{
    m_Index = index;
}
unsigned int LightMap::GetIndex()
{
    return m_Index;
}