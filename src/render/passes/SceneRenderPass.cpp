#include "SceneRenderPass.h"
#include "../../global/GlobalData.h"
#include "../../geometry/Empty.h"

Scene *SceneRenderPass::GetScene() const
{
    return Global::scene;
}
std::vector<ShadedEmptyTransform *> & SceneRenderPass::GetGeos() const
{
    return Global::scene->GetGeos();
}

void SceneRenderPass::SetClipPlane(glm::vec4 clipPlane) const
{
    float height = m_Buffer[0];
    glm::mat4 mat = glm::mat4(0);
    memcpy(&mat, &clipPlane, sizeof(glm::vec4));
    MatricesUniformBufferBindings::FeedMatrices(mat, 2);
}

void SceneRenderPass::Render(RenderContext &rc) 
{
    GetScene()->RenderEnv(rc);
    for (auto *g: GetGeos())
    {
        RenderPass *currentPass = g->GetRenderPass()->First();
        while(currentPass != nullptr)
        {
            currentPass->Render(rc);
            currentPass = currentPass->GetNext();
        }
    }

    
}
ShadowRenderPass::ShadowRenderPass()
{
    m_Type = RenderPassType::SHADOW;
}

void ShadowRenderPass::Render(RenderContext &rc) 
{
    for(ShadedEmptyTransform *g : GetGeos())
    {
        if (g->IsGeometry()) 
        {
            GetScene()->GetLightMapRenderer().RenderGeometry(static_cast<Geometry *>(g), rc);
        }
    }
}

ScreenRenderPass::ScreenRenderPass()
{
    m_Type = RenderPassType::SCREEN;
}
void ScreenRenderPass::Render(RenderContext &rc) 
{
    SceneRenderPass::Render(rc);
}

PostProcessingRenderPass::PostProcessingRenderPass()
{
    m_Type = RenderPassType::POSTPROCESS;
}
void PostProcessingRenderPass::Render(RenderContext &rc) 
{}

ReflectionRenderPass::ReflectionRenderPass()
{
    m_Type = RenderPassType::REFLECTION;
}

ReflectionRenderPass::ReflectionRenderPass(FrameBuffer *fb)
{
    m_Type = RenderPassType::REFLECTION;
    m_FrameBuffer = fb;
}
void ReflectionRenderPass::Render(RenderContext &rc) 
{
    if (m_FrameBuffer == nullptr)
    {
        throw UnsupportedOperationException("frame buffer must be set before render");
    }
    m_FrameBuffer->Bind();
    float reflectionHeight = m_Buffer[0];

    RenderContext _rc = GetScene()->InverseCamera(reflectionHeight);
    glEnable(GL_CLIP_DISTANCE0);
    SetClipPlane(glm::vec4(0.0f, 1.0f, 0.0f, reflectionHeight - 0.01));
    SceneRenderPass::Render(_rc);
    GetScene()->ReverseCamera(m_Buffer[0]);
    glDisable(GL_CLIP_DISTANCE0);
    m_FrameBuffer->UnBind();
}

void ReflectionRenderPass::SetFrameBuffer(FrameBuffer *fb)
{
    m_FrameBuffer = fb;
}

RefractionRenderPass::RefractionRenderPass()
{
    m_Type = RenderPassType::REFRACTION;
}

RefractionRenderPass::RefractionRenderPass(FrameBuffer *fb)
{
    m_Type = RenderPassType::REFRACTION;
    m_FrameBuffer = fb;
}

void RefractionRenderPass::Render(RenderContext &rc) 
{
    if (m_FrameBuffer == nullptr)
    {
        throw UnsupportedOperationException("frame buffer must be set before render");
    }

    m_FrameBuffer->Bind();
    float reflectionHeight = m_Buffer[0];
    glEnable(GL_CLIP_DISTANCE0);
    SetClipPlane(glm::vec4(0.0f, -1.0f, 0.0f, reflectionHeight-0.01));
    SceneRenderPass::Render(rc);
    glDisable(GL_CLIP_DISTANCE0);
    m_FrameBuffer->UnBind();
    
}

void RefractionRenderPass::SetFrameBuffer(FrameBuffer *fb)
{
    m_FrameBuffer = fb;
}