#pragma once
#include "VolumeMaterial.h"
class TerrainSDFMaterial: public VolumeMaterial 
{
public:

    TerrainSDFMaterial();
    ~TerrainSDFMaterial();

    void FlushToShader() override;
    void InitAttribs() override;
};