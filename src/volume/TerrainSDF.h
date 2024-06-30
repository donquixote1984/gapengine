#pragma once
#include "Volume.h"
#include "../textures/Texture.h"
class TerrainSDF: public Volume
{
private:
    Texture *m_TerrainNoise;
public:
    TerrainSDF(VolumeMeta meta);
    ~TerrainSDF();
    void PreRender() override;
};