#pragma once
#include "../geometry/Empty.h"
#include "VolumeMeta.hpp"
#include "../ScreenCanvas.h"
class Volume: public ShadedEmptyTransform
{
protected:
    VolumeMeta m_Meta;
    ScreenCanvas m_Canvas;
    float m_CanvasWidth;
    float m_CanvasHeight;
public:
    Volume(VolumeMeta meta);
    ~Volume();
    void InitPasses() override;
    void RenderDrawCall() override;
    void PreRender() override;
};