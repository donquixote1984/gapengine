#include "EmptyRenderer.h"

EmptyRenderer::EmptyRenderer()
{}
EmptyRenderer::EmptyRenderer(RenderContext & renderContext)
{
    rc = renderContext;
}

void EmptyRenderer::OnRenderScript(GLFWwindow *window)
{
    m_Empty->OnUpdateScript(window);
}
void EmptyRenderer::OnRenderMVP(GLFWwindow *window)
{
    //Shader * shader = m_Empty->GetMaterial()->GetShader();
    //shader->Bind();
    //m_IntrinsicShader->Bind();
    //shader->setUniform4m("projection", rc.projection);
    //shader->setUniform4m("view", rc.view);
    //shader->setUniform4m("model", m_Empty->GetModelMat());
    //shader->setUniform3f("viewPos", rc.camPos.x, rc.camPos.y, rc.camPos.z);
    m_Empty->GetModelMat();
}

void EmptyRenderer::OnRender(GLFWwindow *window)
{
    if (!m_Empty->IsVisible())
    {
        return;
    }
    OnRenderScript(window);
    OnRenderMVP(window);
}

void EmptyRenderer::OnRenderEmpties(std::vector<EmptyTransform *> &empties, GLFWwindow *window)
{
    for (EmptyTransform* e : empties)
    {
        //SetGeo(g);
        m_Empty = e;
        OnRender(window);
    }
}