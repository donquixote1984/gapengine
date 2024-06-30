#pragma once
#include <glm/glm.hpp>
#include "Terrain.h"
class TerrainCPU: public Terrain
{
protected:
    void GenTerrainMesh() override;
    glm::vec3 GetNormal(int x, int y);
    void GetTangent(int x, int y, glm::vec3 &tangent, glm::vec3 &bitangent);
    unsigned char SampleHeight(unsigned char *data, int x, int y, int texWidth, int texHeight, int channels);
    virtual void PreRender() override;
    virtual void ProcessData() override;
public:
    TerrainCPU(TerrainMeta meta);
};