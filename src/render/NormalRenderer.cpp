#include "NormalRenderer.h"

NormalRenderer::NormalRenderer()
{
    m_Shader = new Shader("res/shaders/debug/normal.shader");
}
NormalRenderer::~NormalRenderer()
{
    delete m_Shader;
}
void NormalRenderer::OnRender(GLFWwindow *window)
{
    m_Shader->Bind();
    OnRenderMVP(window);
    m_Geo->RenderDrawCall();
}

 void NormalRenderer::OnRenderMVP(GLFWwindow *window)
 {
    m_Shader->setUniform4m("model", m_Geo->GetModelMat());
    //m_Shader->setUniform3f("viewPos", rc.camPos.x, rc.camPos.y, rc.camPos.z);
 }

 void NormalRenderer::OnRenderGeos(std::vector<Geometry*> &geos, GLFWwindow *window)
{
    for (Geometry* g : geos)
    {
        SetGeo(g);
        OnRender(window);
    }
}

void NormalRenderer::OnRenderGeo(Geometry*geo, GLFWwindow *window)
{
    SetGeo(geo);
    OnRender(window);
}

void NormalRenderer::SetGeo(Geometry * geo)
{
    m_Geo = geo;
}