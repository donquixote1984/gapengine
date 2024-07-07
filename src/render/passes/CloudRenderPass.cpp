#include "CloudRenderPass.h"
CloudRenderPass::CloudRenderPass(Cloud *cloud)
{
    m_Cloud = cloud;
}

void CloudRenderPass::Render(RenderContext &rc)
{
    OnRenderMaterial(rc);
    m_Cloud->PreRender();
    m_Cloud->RenderDrawCall(); // render to it's own frame buffer
}

void CloudRenderPass::OnRenderMaterial(RenderContext &rc) const
{
    Material * mat = m_Cloud -> GetMaterial()[0];
    mat->FlushToShader();
}

