#pragma once
#include "Volume.h"
#include "../Bufferable.h"
#include "../textures/Texture.h"
class Cloud: public Volume
{
private:
    unsigned int m_NoiseChannel = 1;
    unsigned int m_Frame;
    Texture * m_DitherNoise = nullptr;
public:
    Cloud(VolumeMeta meta);
    ~Cloud();
    void InitPasses() override;    
    unsigned int GetNoiseChannel();
    void PreRender() override;
};