#include "GeometryRenderer.h"
#include "passes/RenderPass.h"
#include "passes/RenderPassLinkList.h"

GeometryRenderer::GeometryRenderer()
{}
GeometryRenderer::GeometryRenderer(RenderContext & renderContext)
{
    rc = renderContext;
}

void GeometryRenderer::OnRender(GLFWwindow *window)
{
    if (!m_Geo->IsVisible())
    {
        return;
    }
    m_Geo->OnUpdateScript(window);
    RenderPass *currentPass = m_Geo->GetRenderPass()->First();

    while(currentPass != nullptr)
    {
        currentPass->Render(rc);
        currentPass = currentPass->GetNext();
    }
}


void GeometryRenderer::OnRenderMisc(GLFWwindow *window)
{
    Shader * shader = m_Geo->GetMaterial()->GetShader();
   
    if (m_EnvBox != nullptr) 
    {
        m_EnvBox->ActiveEnvironment();
    }
    m_Geo->PreRender();
}

void GeometryRenderer::OnRenderMaterial()
{
    Material * mat = m_Geo-> GetMaterial();
    mat->FlushToShader();
    mat->FlushToGlobalSettings();
}

void GeometryRenderer::SetGeo(Geometry * geo)
{
    m_Geo = geo;
}

void GeometryRenderer::OnRenderGeos(std::vector<Geometry*> &geos, GLFWwindow *window)
{
    for (Geometry* g : geos)
    {
        SetGeo(g);
        OnRender(window);
    }
}

void GeometryRenderer::OnRenderGeo(Geometry*geo, GLFWwindow *window)
{
    SetGeo(geo);
    OnRender(window);
}

void GeometryRenderer::SetEnvBox(EnvBox * envbox)
{
    m_EnvBox = envbox;
}