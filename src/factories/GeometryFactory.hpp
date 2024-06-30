#pragma once
#include <tuple>
#include <memory>
#include <filesystem>
#include <nlohmann/json.hpp>
#include "magic_enum.hpp"
#include "../reader/ObjReader.h"
#include "../reader/AnimationReader.h"
#include "../primitives/PrimitiveType.h"
#include "../primitives/PrimitiveFactory.h"
#include "../exceptions/InvalidTypeException.h"
#include "../exceptions/MissingConfigurationException.h"
#include "../lights/Light.h"
#include "../primitives/Primitive.h"
#include "../primitives/PrimitiveType.h"
#include "../primitives/PrimitiveFactory.h"
#include "../JsonGeoTypeMapper.h"
#include "../textures/TextureType.h"
#include "../render/DefaultMaterial.h"
#include "../util/JsonUtil.h"
#include "../node/MaterialNodeReader.hpp"
#include "../node/MaterialTemplate.h"
#include "../node/MaterialTemplateType.h"
#include "../render/CustomMaterial.h"
#include "../geometry/postprocess/DataPostProcessingFactory.h"
#include "../geometry/Empty.h"
#include "../geometry/GeometryMeta.h"
#include "../volume/Volume.h"
#include "../volume/VolumeMeta.hpp"
#include "../terrain/Terrain.h"
#include "../water/Water.h"
#include "AssetHandler.h"




std::string ReadTexture(const nlohmann::json &textureJson, std::string key)
{
    try {
        std::string texturePath = textureJson.at(key);
        return texturePath;
    } catch (const nlohmann::json::exception& e)
    {
        return "";
    }
}

class GeometryFactory
{
public:
    virtual ~GeometryFactory() {}
    virtual EmptyTransform * CreateGeometry(const nlohmann::json &geojson) = 0;
  
    EmptyTransform * PresolveGeometryScript(EmptyTransform *g, const nlohmann::json &geojson)
    {
        if (geojson.contains("script"))
        {
            std::string script;
            nlohmann::json j = geojson["script"];
            if (j.contains("file"))
            {
                const std::string scriptPath = j["file"];
                std::string geoBasePath = geojson["basepath"];
                std::filesystem::path geoBasePathfs = geoBasePath;
                std::filesystem::path scriptPathfs = scriptPath;
                geoBasePathfs = geoBasePathfs/scriptPathfs;
                if(!std::filesystem::exists(geoBasePathfs))
                {
                    throw MissingConfigurationException((geoBasePathfs.string() + " does not exists").c_str());
                }

                script = util::ReadFile(geoBasePathfs.string());

            }
            else if (j.contains("string"))
            {
                script = j["string"];
            }

            g->AttachScript(script);
        }
        return g;
    }
    EmptyTransform * PresolveGeometryMVP(EmptyTransform* g, const nlohmann::json &geojson) {
        

        glm::vec3 position = util::GetPosition(geojson["position"]);
        g->SetPosition(position);
        if (geojson.contains("rotate"))
        {
            glm::vec3 axis = util::GetPosition(geojson["rotate"]["axis"]);
            float angle = geojson["rotate"]["angle"];
            g->Rotate(angle, axis);
        }
        if (geojson.contains("scale")) 
        {
            glm::vec3 scale = util::GetPosition(geojson["scale"]);
            g->SetInitScale(scale);
        }

       


        if (geojson.contains("visible"))
        {
            bool visible = geojson["visible"];
            g->SetVisible(visible);
        }
        return g;
    }

    ShadedEmptyTransform *PresolveGeometryMaterial(ShadedEmptyTransform * g, const nlohmann::json &geojson)
    {
        if (geojson.contains("material")) {
            nlohmann::json materialJson = geojson["material"];
            std::shared_ptr<MaterialTemplate> t = MaterialTemplateBuilder::buildTemplate(materialJson);
            std::shared_ptr<MaterialValueNode> mvn = MaterialNodeReader::ReadMaterial(materialJson, t.get());
            Material * df = static_cast<Material*> (mvn.get()->GetFirstOutputValue().GetPtr());
            g->SetMaterial(df);
        } 
        return g;
    }

    Geometry *PresolveGeometryData(Geometry *g, const nlohmann::json &geojson)
    {
        ObjPreProcessing opp;
        if (geojson.contains("preprocessing"))
        {
            opp = util::GetObjPreProcessing(geojson["preprocessing"]);
        }

        std::string geoFilePath;
        if (geojson.contains("data")) {
            geoFilePath = geojson["data"];
            ObjReader r(geoFilePath, opp);
            r >> g;
        }
        else if (geojson.contains("asset")) 
        {
            std::string assetPath = geojson["asset"];
            
            AssetHandler r(assetPath, opp);
            r >> g;
            r >> static_cast<BasicMaterial *>(g->GetMaterial());
        }
        
        if (geojson.contains("animation"))
        {
            std::string assetPath = geojson["animation"];
            AnimationReader ar(assetPath, opp);
            ar >> g;
        }
        if (geojson.contains("mode"))
        {
            DisplayMode mode = util::GetDisplayMode(geojson["mode"]);
            g->SetDisplayMode(mode);
        }
        if (geojson.contains("autoplay"))
        {
            bool autoplay = geojson["autoplay"];
            g->SetPlaying(true);
        }
        if(geojson.contains("postprocessing"))
        {
            //for (auto & pp: geojson["postprocessing")
            if (geojson["postprocessing"].type() == nlohmann::json::value_t::array)
            {
                std::vector<std::string> pp = geojson["postprocessing"];

                for (std::string &p : pp)
                {
                    PostProcessingType type = util::GetPPType(p);
                    DataPostProcessing* dpp = DataPostProcessingFactory::GetPostProcessing(type);
                    g->AddPostProcessing(dpp);
                }
            } else {
                std::string pp = geojson["postprocessing"];
                PostProcessingType type = util::GetPPType(pp);
                DataPostProcessing* dpp = DataPostProcessingFactory::GetPostProcessing(type);
                g->AddPostProcessing(dpp);
            }
        }

        g->ProcessData();
        g->InitPasses();
        return g;
    }
};


class ContainerGeometryFactory: public GeometryFactory
{
public:
    EmptyTransform * CreateGeometry(const nlohmann::json &geojson) override
    {
        GeometryMeta meta;
        meta.geoType = GeometryType::CONTAINER;
        EmptyTransform *g = Global::CreateGeometry(GeometryType::CONTAINER, geojson["name"], meta);//new Geometry(GeometryType::MESH);

        if (geojson.contains("parent"))
        {
            std::string parentStr = geojson["parent"];
            EmptyTransform *parent = Global::GeometryPool[parentStr];
            g->AttachParent(parent);
        }
        PresolveGeometryMVP(g, geojson);
        PresolveGeometryScript(g, geojson);
        return g;
    }
};
class TerrainGeometryFactory: public GeometryFactory
{
    Geometry* CreateGeometry(const nlohmann::json &geojson) override
    {
        //Geometry *g;
        TerrainMeta meta;
        if (geojson.contains("meta"))
        {
            meta = TerrainMeta::FromJson(geojson["meta"]);
        }

        EmptyTransform * _g = Global::CreateTerrain(meta);
        Geometry *g = static_cast<Geometry *>(_g);
        PresolveGeometryMVP(g, geojson);
        PresolveGeometryMaterial(g, geojson);
        PresolveGeometryData(g, geojson);
        PresolveGeometryScript(g, geojson);
        return g;
    }
};

class WaterGeometryFactory: public GeometryFactory
{
    Geometry* CreateGeometry(const nlohmann::json &geojson) override
    {
        GeometryMeta meta;
        Geometry *p = Global::CreateWater();
        PresolveGeometryMVP(p, geojson);
        PresolveGeometryMaterial(p, geojson);
        PresolveGeometryData(p, geojson);
        PresolveGeometryScript(p, geojson);
        return p;
    }
};

class VolumeGeometryFactory: public GeometryFactory
{
    EmptyTransform * CreateGeometry(const nlohmann::json &geojson) override
    {
        VolumeMeta meta;
        if (geojson.contains("meta"))
        {
            meta = VolumeMeta::FromJson(geojson["meta"]);
            meta.name = geojson["name"];
        }
        ShadedEmptyTransform *v = Global::CreateVolume(meta);
        PresolveGeometryMVP(v, geojson);
        PresolveGeometryMaterial(v, geojson);
        v->InitPasses();
        return v;
    }
};

class MeshGeometryFactory: public GeometryFactory 
{
public:
    void ResolveAsset(const std::string &assetpath)
    {
        //AssetHandler ah(assetpath);
        
    }

    Geometry* CreateGeometry(const nlohmann::json &geojson) override
    {

        //Geometry *g;
        if (!(geojson.contains("data") || geojson.contains("asset")))
        {
            throw MissingConfigurationException("no 'data' or 'asset' value for geometry");
        }

        std::string type = geojson["type"];

        GeometryMeta meta ;
        if (geojson.contains("meta")) {
            meta = GeometryMeta::FromJson(geojson["meta"]);
        }
        //ValidateMeta(meta, g);
        EmptyTransform *_g = Global::CreateGeometry(GeometryType::MESH, geojson["name"], meta);//new Geometry(GeometryType::MESH);
        // instancing -> type
        Geometry *g = static_cast<Geometry *>(_g);

        if (geojson.contains("parent"))
        {
            std::string parentStr = geojson["parent"];
            EmptyTransform *parent = Global::GeometryPool[parentStr];
            g->AttachParent(parent);
        }

        //TexturedGeometry *tg = new TexturedGeometry(geoFilePath);
        PresolveGeometryMVP(g, geojson);
        PresolveGeometryMaterial(g, geojson);
        PresolveGeometryData(g, geojson);
        PresolveGeometryScript(g, geojson);
        //return tg;
        return g;
    }
};

class PrimitiveGeometryFactory: public GeometryFactory
{
    Geometry * CreateGeometry(const nlohmann::json &geojson) override
    {
        if (!geojson.contains("subtype"))
        {
            throw MissingConfigurationException("scene file need a subtype for primitive");
        }
        
        GeometryMeta meta;
        if (geojson.contains("meta")) {
            meta = GeometryMeta::FromJson(geojson["meta"]);
        }
        std::string gtstr = geojson["type"];
        std::string ptstr = geojson["subtype"];
        auto [geoType, subType] = JsonGeoTypeMapper::GetType(gtstr, ptstr);
        Geometry *p = Global::CreatePrimitive(static_cast<PrimitiveType>(subType), meta); //PrimitiveFactory::CreatePrimitive(static_cast<PrimitiveType>(subType));

        if (geojson.contains("parent"))
        {
            std::string parentStr = geojson["parent"];
            EmptyTransform * parent = Global::GeometryPool[parentStr];
            p->AttachParent(parent);
        }
        PresolveGeometryMVP(p, geojson);
        PresolveGeometryMaterial(p, geojson);
        PresolveGeometryData(p, geojson);
        PresolveGeometryScript(p, geojson);
        return p;
    }
};