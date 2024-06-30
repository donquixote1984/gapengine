#include "GlobalData.h"
#include "../exceptions/UnsupportedOperationException.h"


namespace Global
{
    Scene * scene;
    UI * ui;
    Camera * camera;
    std::unordered_map<std::string, GeometryData*> geometryDataMap;
    std::unordered_map<std::string, Texture *> texturesMap;
    std::unordered_map<std::string, EmptyTransform*> GeometryPool;
    std::unordered_map<std::string, SceneAnimation *> animationMap;
}


void Global::FreeGlobalData()
{
    for (auto &kv: Global::geometryDataMap)
    {
        delete kv.second;
    }

    for (auto &kv: Global::texturesMap)
    {
        delete kv.second;
    }
}

EmptyTransform * Global::CreateTerrain(TerrainMeta meta)
{
    Terrain * t;
    if (meta.renderType == TerrainRenderType::CPU) {
        t = new TerrainCPU(meta);
    } else {
        t = new TerrainGPU(meta);
    }
    t->InitProcessor();
    return t;
}
EmptyTransform * Global::CreateGeometry(GeometryType type, const std::string &name, GeometryMeta meta)
 {
    EmptyTransform * g = nullptr;
    if (type == GeometryType::CONTAINER) 
    {
        g = new EmptyTransform();
    } else 
    {
        Geometry *_g = nullptr;
        if (meta.instancing.avail) {
            _g = new GeometryInstancing(type);
        } else {
            _g  = new Geometry(type);
        }
        
        _g->AttachMeta(meta);
        _g->InitProcessor();
        g = _g;
    }
    
    g->SetName(name);
    if (type != GeometryType::CONTAINER) {
        
    }
    GeometryPool[name] = g;
    return g;
 }

Geometry * Global::CreateWater()
{
    Water *w = new Water();
    w->InitProcessor();
    GeometryPool[w->GetName()]= w;
    return w;
}

ShadedEmptyTransform *Global::CreateVolume(VolumeMeta &meta)
{
    ShadedEmptyTransform *vol = nullptr;
    if (meta.type == VolumeType::SDF)
    {
        vol = new Volume(meta);
    }
    else if (meta.type == VolumeType::SIMPLE_CLOUD)
    {
        vol = new SimpleCloud(meta);
    }
    else if (meta.type == VolumeType::TERRAIN)
    {
        vol = new TerrainSDF(meta);
    }
    else if (meta.type == VolumeType::CLOUD)
    {
        vol = new Cloud(meta);
    }
    else {
        throw UnsupportedOperationException("Invalid volume type");
    }
    GeometryPool[vol->GetName()] = vol;
    return vol;
}

Geometry* Global::CreatePrimitive(PrimitiveType type, GeometryMeta meta){
    Geometry * p = nullptr;
    if (meta.instancing.avail)
    {
        p = PrimitiveInstancingFactory::CreatePrimitiveInstancing(type);
    } else {
        p = PrimitiveFactory::CreatePrimitive(type);
    }
    
    std::string name = p->GetName();
    p->AttachMeta(meta);
    p->InitProcessor();
    GeometryPool[name] = p;
    return p;
 }