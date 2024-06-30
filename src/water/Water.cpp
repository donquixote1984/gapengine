#include "Water.h"
#include "../global/ScreenProps.h"
#include "../global/GlobalData.h"
#include "../Camera.h"
#include "../FrameBufferDebugger.h"
#include "../render/passes/SceneRenderPass.h"
#include "../render/passes/RenderPassLinkList.h"
#include "../FrameBufferDebugger.h"
#include <GLFW/glfw3.h>
Water::Water()
{
    m_ReflectionFrameBuffer = new FrameBuffer(Global::screen.width, Global::screen.height);
    m_RefractionFrameBuffer = new FrameBuffer(Global::screen.width, Global::screen.height);
    FrameBufferDebugger::get()->SetCurrentFrameBuffer(m_ReflectionFrameBuffer);
    Water::counter+= 1;
    this->m_Name = "water." + std::to_string(Water::counter);
    this->m_Type = GeometryType::WATER;
}

Water::~Water()
{
    delete m_ReflectionFrameBuffer;
    delete m_RefractionFrameBuffer;
}

void Water::RenderFrameBuffer()
{
    RenderReflectionFrameBuffer();
    RenderRefractionFrameBuffer();
}


void Water::RenderReflectionFrameBuffer()
{
    glEnable(GL_CLIP_DISTANCE0);
    Camera *c = Global::camera;
    c->InverseY(m_Height);
    m_ReflectionFrameBuffer->Bind();
    m_ReflectionFrameBuffer->Render();
    m_ReflectionFrameBuffer->UnBind();
    c->ReverseY(m_Height);
    glDisable(GL_CLIP_DISTANCE0);
}

void Water::RenderRefractionFrameBuffer()
{}

void Water::InitPasses()
{
    m_RenderPasses = new RenderPassLinkList();
    //m_RenderPasses->AppendRenderPass(RenderPassLinkList::CreateRenderPass(RenderPassType::REFLECTION, this));
    //m_RenderPasses->AppendRenderPass(RenderPassLinkList::CreateRenderPass(RenderPassType::REFRACTION, this));
    m_RenderPasses->AppendRenderPass(RenderPassLinkList::CreateRenderPass(RenderPassType::GEOMETRY, this));
}

void Water::PreRender()
{
    m_ReflectionFrameBuffer->ActiveTexture(constants::REFLECTION_TEXTURE);
    m_RefractionFrameBuffer->ActiveTexture(constants::REFRACTION_TEXTURE);
    
}

void Water::SetScene(Scene *s)
{
    EmptyTransform::SetScene(s);
    // add frame buffer render pass to scene
    ReflectionRenderPass * rflpass = static_cast<ReflectionRenderPass *>(RenderPassLinkList::CreateSceneRenderPass(RenderPassType::REFLECTION));
    rflpass->SetFrameBuffer(m_ReflectionFrameBuffer);
    s->AppendPreRenderPass(rflpass);
    rflpass->Buffer(m_Height, 0);
    RefractionRenderPass* rfrpass = static_cast<RefractionRenderPass *>(RenderPassLinkList::CreateSceneRenderPass(RenderPassType::REFRACTION));
    rfrpass->SetFrameBuffer(m_RefractionFrameBuffer);
    s->AppendPreRenderPass(rfrpass);
    rfrpass->Buffer(m_Height, 0);
    //FrameBufferDebugRenderPass * fbdpass = static_cast<FrameBufferDebugRenderPass *>(RenderPassLinkList::CreateSceneRenderPass(RenderPassType::FRAME_BUFFER_DEBUG));
    //s->AppendPostRenderPass(fbdpass);
    //fbdpass->SetCurrentFrameBuffer(m_ReflectionFrameBuffer);
}

int Water::counter = 0;