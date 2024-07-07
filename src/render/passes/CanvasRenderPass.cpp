#include "CanvasRenderPass.h"
#include "../../geometry/Empty.h"
#include "../../global/ScreenProps.h"

CanvasRenderPass::CanvasRenderPass(ShadedEmptyTransform *geo)
{
    m_Geo = geo;
}

void CanvasRenderPass::Render(RenderContext &rc) 
{
    OnRenderMaterial(rc);
    //Global::atomsphere->Bind();
    m_Geo->PreRender();
    m_Geo->RenderDrawCall();

}

void CanvasRenderPass::OnRenderMaterial(RenderContext &rc) const
{
    //Material * mat = m_Geo-> GetMaterial();
    
    //mat->FlushToShader();
    //mat->FlushToGlobalSettings();
    
}