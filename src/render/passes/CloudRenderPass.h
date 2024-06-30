#pragma once
#include "RenderPass.h"
#include "../../volume/Cloud.h"
class CloudRenderPass: public RenderPass
{
private:
    Cloud * m_Cloud = nullptr;
    void OnRenderMaterial(RenderContext &rc) const;
public:
    CloudRenderPass(Cloud *c);
    void Render(RenderContext &rc) override;
};