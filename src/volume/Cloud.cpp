#include "Cloud.h"
#include "../render/passes/RenderPassLinkList.h"
#include "../FrameBufferDebugger.h"
#include "../global/GlobalData.h"
#include "../Config.h"
Cloud::Cloud(VolumeMeta meta): Volume(meta)
{
    m_DitherNoise = new Texture("res/textures/noise/bluenoise3.png", false, false);
}
Cloud::~Cloud()
{
    delete m_DitherNoise;
}

void Cloud::InitPasses()
{
    m_RenderPasses = new RenderPassLinkList();
    RenderPass * cloudrp = RenderPassLinkList::CreateRenderPass(RenderPassType::CANVAS, this);
    //rp->GetFrameBuffer();
    m_RenderPasses->AppendRenderPass(cloudrp);
       

}
void Cloud::PreRender()
{
    m_Frame += 1;
    //Shader *s = this->GetMaterial()->GetShader();
    //s->Bind();
    this->GetUniforms().Cache("u_Resolution", m_CanvasWidth, m_CanvasHeight);
    this->GetUniforms().Cache("u_Frame", m_Frame);
    m_DitherNoise->Bind(2);
    this->GetUniforms().Cache("u_BlueNoiseTexture", 2);
    //this->Bind(m_CanvasWidth, m_CanvasHeight); // bind to frame buffer
    //glViewport(0, 0, m_CanvasHeight, m_CanvasHeight);
}

unsigned int Cloud::GetNoiseChannel()
{
    return m_NoiseChannel;
}