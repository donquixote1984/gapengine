#include "RenderPassLinkList.h"
#include "GeometryRenderPass.h"
#include "NoiseRenderPass.h"
#include "SceneRenderPass.h"
#include "CanvasRenderPass.h"
#include "BicubicRenderPass.h"
#include "CloudNoiseLutRenderPass.h"
#include "FrameCacheRenderPass.h"
#include "CloudRenderPass.h"
#include "../../FrameBufferDebugger.h"

void RenderPassLinkList::AppendRenderPass(RenderPass* renderPass)
{
    if (renderPass == nullptr)
    {
        return;
    }
    if (m_Size == 0)
    {
        m_Head = m_Tail = renderPass;
        
    } else {
        m_Tail->AppendNext(renderPass);
        m_Tail = renderPass;
    }
    m_Size += 1;
}
RenderPassLinkList::~RenderPassLinkList()
{
    delete m_Head;
}

void RenderPassLinkList::PrependRenderPass(RenderPass * renderPass)
{
    renderPass->AppendNext(m_Head);
    m_Head = renderPass;
}

RenderPass * RenderPassLinkList::First()
{
    return m_Head;
}
RenderPass* RenderPassLinkList::CreateSceneRenderPass(RenderPassType type)
{
    switch (type)
    {
        case RenderPassType::SCREEN:
            return new ScreenRenderPass();
        case RenderPassType::POSTPROCESS:
            return new PostProcessingRenderPass();
        case RenderPassType::SHADOW:
            return new ShadowRenderPass();
        case RenderPassType::REFLECTION:
            return new ReflectionRenderPass();
        case RenderPassType::REFRACTION:
            return new RefractionRenderPass();
        case RenderPassType::FRAME_BUFFER_DEBUG:
            return new FrameBufferDebugRenderPass();
        default:
            throw UnsupportedOperationException("invalid render pass type");
    }
}

RenderPass * RenderPassLinkList::CreateRenderPass(RenderPassType type, ShadedEmptyTransform *g)
{
    switch (type)
    {
        case RenderPassType::GEOMETRY:
            return new GeometryRenderPass(static_cast<Geometry*>(g));
        case RenderPassType::NOISE:
            return new NoiseRenderPass();
        case RenderPassType::CANVAS:
            return new CanvasRenderPass(g);
        case RenderPassType::CLOUD_NOISE:
            return new CloudNoiseLutRenderPass();
        case RenderPassType::CLOUD:
            return new CloudRenderPass(static_cast<Cloud *> (g));
        default:
            return nullptr;
    }
}

RenderPass * RenderPassLinkList::CreateFrameBufferRenderPass(RenderPassType type, Bufferable *g)
{
    switch (type)
    {
        case RenderPassType::BICUBIC:
            return new BicubicRenderPass(g);
        case RenderPassType::FRAME_CACHE:
            return new FrameCacheRenderPass(g);
        default:
            return nullptr;
    }
}

RenderPass * RenderPassLinkList::GetFirstByType(RenderPassType type)
{
    RenderPass *current = m_Head;
    while(current != nullptr)
    {
        if (current->GetType() == type)
        {
            return current;
        }
        current = current->GetNext();
    }

    return nullptr;
}

void RenderPassLinkList::AppendToFirstType(RenderPassType type, RenderPass * pass)
{
    RenderPass *current = RenderPassLinkList::GetFirstByType(type);
    pass->AppendNext(current->GetNext());
    current->AppendNext(pass);
}

void RenderPassLinkList::PrependToFirstType(RenderPassType type, RenderPass * pass)
{
    RenderPass *current = m_Head;
    if (current->GetType() == type)
    {
        pass->AppendNext(m_Head);
        m_Head = pass;
        return;
    }
    while(current->GetNext() != nullptr)
    {
        if (current->GetNext()->GetType() == type)
        {
            pass->AppendNext(current->GetNext());
            current->AppendNext(pass);
            return;
        }
        current = current->GetNext();
    }
}

void RenderPassLinkList::Render(RenderContext &rc) const
{
    RenderPass *pass = m_Head;
    while(pass != nullptr)
    {
        pass->Render(rc);
        pass = pass->GetNext();
    }
}