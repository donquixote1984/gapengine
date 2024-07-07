#include "SimpleCloud.h"
#include "../render/passes/RenderPassLinkList.h"
#include "../global/ScreenProps.h"
#include "../global/GlobalData.h"

#include "../FrameBufferDebugger.h"
SimpleCloud::SimpleCloud(VolumeMeta meta): Volume(meta), Bufferable(meta.canvasWidth/2, meta.canvasHeight/2)
{
    m_BlueNoiseTexture = new Texture("res/textures/noise/bluenoise2.png", false, true);
    m_DitherNoise = new Texture("res/textures/noise/bluenoise.png", false, true);
}

SimpleCloud::~SimpleCloud()
{
    delete m_BlueNoiseTexture;
    delete m_DitherNoise;
}

void SimpleCloud::PreRender()
{
    m_Frame += 1;
    //Shader *s = this->GetMaterial()->GetShader();
    //s->Bind();
    m_BlueNoiseTexture->Bind(0);
    m_DitherNoise->Bind(1);
    this->GetUniforms().Cache("u_NoiseTexture", 0);
    this->GetUniforms().Cache("u_DitherTexture", 1);
    this->GetUniforms().Cache("u_Frame", m_Frame);
    this->GetUniforms().Cache("u_Resolution", m_CanvasWidth/2, m_CanvasHeight/2);
    this->Bind(m_CanvasWidth/2, m_CanvasHeight/2); // bind to frame buffer
}
void SimpleCloud::InitPasses()
{
    m_RenderPasses = new RenderPassLinkList();
    m_RenderPasses->AppendRenderPass(RenderPassLinkList::CreateRenderPass(RenderPassType::CANVAS, this));
    m_RenderPasses->AppendRenderPass(RenderPassLinkList::CreateFrameBufferRenderPass(RenderPassType::BICUBIC, this));
   
    //FrameBufferDebugRenderPass * fbdpass = static_cast<FrameBufferDebugRenderPass *>(RenderPassLinkList::CreateSceneRenderPass(RenderPassType::FRAME_BUFFER_DEBUG));
    //Global::scene->AppendPostRenderPass(fbdpass);
    //fbdpass->SetCurrentFrameBuffer(this->GetFrameBuffer());
}

void SimpleCloud::RenderDrawCall()
{
    m_Canvas.Draw();
}