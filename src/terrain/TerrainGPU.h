#pragma once
#include "Terrain.h"
#include "../textures/Texture.h"
#include "../alg/NoiseFrameBuffer.h"
#include "../global/ScreenProps.h"

class TerrainGPU: public Terrain
{
private:
    Texture *m_Texture = nullptr;
    NoiseFrameBuffer *m_NoiseFrameBuffer = nullptr;
protected:
    void GenTerrainMesh() override;
    void ProcessData() override;
    virtual void PreRender() override;
public:
     TerrainGPU(TerrainMeta meta);
    ~TerrainGPU();
    void InitPasses() override;
};