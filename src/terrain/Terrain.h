#pragma once
#include <glm/glm.hpp>
#include <memory>
#include "../shaders/Shader.h"
#include "../geometry/Geometry.h"
#include "../Config.h"
#include <nlohmann/json.hpp>
#include "../util/JsonUtil.h"
#include "TerrainDataProcessor.h"
#include "../geometry/GeometryDataProcessor.h"
#include "../exceptions/MissingConfigurationException.h"

using namespace nlohmann;
enum class TerrainRenderType
{
    CPU,
    GPU
};

struct TerrainMeta
{
    TerrainRenderType renderType = TerrainRenderType::CPU;
    int res = 1024;
    int size = 1024;
    std::string heightmap;
    bool useNoise;
    glm::vec4 range = {.0f, 1.0f, .0f, 1.0f};
    glm::vec2 animated = {0.0f, .0f};
    bool averageHeight = false;
    static TerrainMeta FromJson(json j)
    {
        TerrainMeta meta;
        if (j.contains("type"))
        {
            std::string type = j["type"];
            if (type == "GPU")
            {
                meta.renderType = TerrainRenderType::GPU;

            } else {
                meta.renderType = TerrainRenderType::CPU;
            }
        }

        if (j.contains("res"))
        {
            json res = j["res"];

            if (meta.renderType == TerrainRenderType::GPU)
            {
                meta.res = res["gpu"];
                if (meta.res > constants::MAX_TERRAIN_RES_GPU)
                {
                    throw MissingConfigurationException("too large for terrain meta resolution");
                }
            }

            if (meta.renderType == TerrainRenderType::CPU)
            {
                meta.res = res["cpu"];
                if (meta.res > constants::MAX_TERRAIN_RES_CPU)
                {
                    throw MissingConfigurationException("too large for terrain meta resolution");
                }
            }
        }
        if (j.contains("averageHeight"))
        {
            meta.averageHeight = j["averageHeight"];
        }

        if (j.contains("size"))
        {
            int size = j["size"];
            meta.size = size;
        }

        if (j.contains("noise"))
        {
            bool useNoise = j["noise"];
            meta.useNoise = useNoise;
        }

        if (j.contains("heightmap"))
        {
            std::string heightmap = j["heightmap"];
            meta.heightmap = heightmap;
        }
        if (j.contains("from"))
        {
            glm::vec3 from = util::GetPosition(j["from"]);
            meta.range[0] = from[0];
            meta.range[1] = from[1];
        }

        if (j.contains("to"))
        {
            glm::vec3 to = util::GetPosition(j["to"]);
            meta.range[2] = to[0];
            meta.range[3] = to[1];
        }

        if (j.contains("animated"))
        {
            glm::vec3 ani = util::GetPosition(j["animated"]);
            meta.animated = {ani.x, ani.y};
        }

        return meta;
    }
};

class Terrain: public Geometry
{
protected:
    Mesh m_Mesh;
    Shader * m_Shader = nullptr;
    TerrainMeta m_Meta;
    virtual void GenTerrainMesh() = 0;
    void GenIndices();
    unsigned int GetIndicesIndex(int i, int j);
    glm::vec3 GetGridPoints(int x, int y);
    float FitSize(float x);
    const int m_Unit = 4;
    glm::vec2 m_Offset = {0, 0};
public:
    Terrain();
    Terrain(TerrainMeta meta);
    virtual ~Terrain();
    static int TerrainCounter;
    void AttachMeta(TerrainMeta meta);
    void InitProcessor() override;
    virtual void PreRender() override;
    TerrainMeta & GetTerrainMeta();
    int GetTessUnit();
    bool HasTess() override;
    void ReadyToScene() override;
};