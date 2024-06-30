#pragma once
#include "../primitives/Plane.h"
#include "../FrameBuffer.h"

struct WaterMeta
{

};
class Water: public Plane
{
private:
    float m_Height = 0.0f;
    FrameBuffer *m_ReflectionFrameBuffer;
    FrameBuffer *m_RefractionFrameBuffer;
    void RenderReflectionFrameBuffer();
    void RenderRefractionFrameBuffer();
    static int counter;
public:
    Water();   
    ~Water();
    void InitPasses() override;
    void RenderFrameBuffer();
    void SetScene(Scene *) override;
    void PreRender() override;
};