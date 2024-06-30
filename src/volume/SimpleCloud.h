#pragma once
#include "../geometry/Empty.h"
#include "VolumeMeta.hpp"
#include "../ScreenCanvas.h"
#include "../textures/Texture.h"
#include "../shaders/Shader.h"
#include "../FrameBuffer.h"
#include "../Bufferable.h"
#include "Volume.h"

class SimpleCloud: public Volume, Bufferable
{
private:
    float density = 1.0;
    int m_Frame = 0;
    
    Texture * m_BlueNoiseTexture = nullptr;
    Texture * m_DitherNoise = nullptr;

public:
    SimpleCloud(VolumeMeta meta);
    ~SimpleCloud();
    void InitPasses() override;
    void RenderDrawCall() override;
    void PreRender() override;
};