#pragma once
#include "RenderPass.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "../RenderContext.h"
#include <vector>
#include "../../FrameBuffer.h"
#include "../../exceptions/UnsupportedOperationException.h"

class Scene;
class ShadedEmptyTransform;

class SceneRenderPass: public RenderPass
{
public:
    void Render(RenderContext &rc) override;
    Scene *GetScene() const;
    std::vector<ShadedEmptyTransform *> & GetGeos() const;
    void SetClipPlane(glm::vec4 clipPlane) const;
};

class ShadowRenderPass: public SceneRenderPass
{
public:
    ShadowRenderPass();
    void Render(RenderContext &rc) override; 
};

class ScreenRenderPass: public SceneRenderPass
{
public:
    ScreenRenderPass();
    void Render(RenderContext &rc) override; 
};

class PostProcessingRenderPass: public SceneRenderPass
{
public:
    PostProcessingRenderPass();
    void Render(RenderContext &rc) override; 
};

class ReflectionRenderPass: public SceneRenderPass
{
private:
   FrameBuffer *m_FrameBuffer = nullptr; 
public:
    ReflectionRenderPass();
    ReflectionRenderPass(FrameBuffer * fb);
   void Render(RenderContext &rc) override; 
   void SetFrameBuffer(FrameBuffer * fb);
};

class RefractionRenderPass: public SceneRenderPass
{
private:
    FrameBuffer * m_FrameBuffer = nullptr;
public:
    RefractionRenderPass();
    RefractionRenderPass(FrameBuffer *fb);
    void Render(RenderContext &rc) override; 
    void SetFrameBuffer(FrameBuffer * fb);
};


