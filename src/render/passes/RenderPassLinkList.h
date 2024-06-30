#pragma once

#include <array>
#include "RenderPassType.h"
#include "RenderPass.h"
#include "../../exceptions/UnsupportedOperationException.h"
#include "../RenderContext.h"
#include "../../geometry/Empty.h"
#include "../../Bufferable.h"

class RenderPass;

class RenderPassLinkList
{
private:
    RenderPass *m_Head = nullptr;
    RenderPass *m_Tail = nullptr;
    int m_Size;
public:
    ~RenderPassLinkList();
    void AppendRenderPass(RenderPass *);
    void PrependRenderPass(RenderPass *);
    
    static RenderPass * CreateRenderPass(RenderPassType type, ShadedEmptyTransform *g);
    static RenderPass * CreateFrameBufferRenderPass(RenderPassType type, Bufferable *g);
    static RenderPass * CreateSceneRenderPass(RenderPassType type);
    RenderPass * First();

    RenderPass * GetFirstByType(RenderPassType type);
    void PrependToFirstType(RenderPassType type, RenderPass * pass);
    void AppendToFirstType(RenderPassType type, RenderPass * pass);

    void Render(RenderContext &rc) const;
};