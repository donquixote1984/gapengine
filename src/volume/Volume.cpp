#include "Volume.h"
#include "../render/passes/RenderPassLinkList.h"
#include "../global/ScreenProps.h"
#include "../global/GlobalData.h"

#include "../FrameBufferDebugger.h"
Volume::Volume(VolumeMeta meta): m_Meta(meta)
{
    this->m_Name = meta.name;
    this->m_CanvasHeight = meta.canvasHeight;
    this->m_CanvasWidth = meta.canvasWidth;
}

Volume::~Volume()
{
}

void Volume::PreRender()
{
    Shader *s = this->GetMaterial()->GetShader();
    s->Bind();
    s->setUniform2f("u_Resolution", m_CanvasWidth, m_CanvasHeight);
    glViewport(0, 0, m_CanvasWidth, m_CanvasHeight);
}
void Volume::InitPasses()
{
    m_RenderPasses = new RenderPassLinkList();
    m_RenderPasses->AppendRenderPass(RenderPassLinkList::CreateRenderPass(RenderPassType::CANVAS, this));

}

void Volume::RenderDrawCall()
{
    m_Canvas.Draw();
}