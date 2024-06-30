#pragma once
#include <unordered_map>
#include "../geometry/GeometryData.h"
#include "../textures/Texture.h"
#include "../Scene.h"
#include "../Camera.h"
#include "../ui/UI.h"
#include "../geometry/GeometryType.h"
#include "../primitives/PrimitiveFactory.h"
#include "../primitives/PrimitiveInstancingFactory.h"
#include "../primitives/PrimitiveType.h"
#include "../exceptions/GeometryInitializeException.h"
#include "../geometry/Geometry.h"
#include "../geometry/Empty.h"
#include "../animation/SceneAnimation.h"
#include "../terrain/Terrain.h"
#include "../terrain/TerrainCPU.h"
#include "../terrain/TerrainGPU.h"
#include "../water/Water.h"
#include "../geometry/GeometryMeta.h"
#include "../volume/VolumeMeta.hpp"
#include "../volume/Volume.h"
#include "../volume/SimpleCloud.h"
#include "../volume/Cloud.h"
#include "../volume/TerrainSDF.h"
#include "../geometry/GeometryInstancing.h"

class Scene;
namespace Global
{
    extern Scene * scene;
    extern Camera *camera;
    extern UI *ui;
    extern std::unordered_map<std::string, GeometryData*> geometryDataMap;
    extern std::unordered_map<std::string, Texture *> texturesMap;
    extern std::unordered_map<std::string, EmptyTransform*> GeometryPool;
    extern std::unordered_map<std::string, SceneAnimation *> animationMap;

    void FreeGlobalData();
    EmptyTransform* CreateGeometry(GeometryType type, const std::string &name, GeometryMeta meta);
    ShadedEmptyTransform * CreateVolume(VolumeMeta &meta);
    Geometry* CreatePrimitive(PrimitiveType type, GeometryMeta meta);
    EmptyTransform* CreateTerrain(TerrainMeta meta);
    Geometry* CreateWater();
};