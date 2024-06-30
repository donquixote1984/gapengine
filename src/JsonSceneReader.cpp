#include <fstream>
#include <iostream>
#include <filesystem>
#include "JsonSceneReader.h"
#include "nlohmann/json.hpp"
#include "primitives/PrimitiveType.h"
#include "factories/GeometryFactory.hpp"
#include "factories/GeometryFactoryBuilder.hpp"
#include "factories/LightFactoryBuilder.hpp"
#include "factories/LightFactory.hpp"
#include "exceptions/InvalidTypeException.h"
#include "exceptions/UnsupportedOperationException.h"
#include "exceptions/ResourceNotFoundException.h"
#include <fmt/format.h>
#include <vector>
#include "JsonGeoTypeMapper.h"
#include <tuple>
#include "operation/Operation.h"
using namespace nlohmann;
WorldGraphNode::WorldGraphNode(const std::string &name)
{
    this->name = name;
}
WorldGraphNode::~WorldGraphNode()
{
    for (auto &c: children)
    {
        delete c;
    }
}

WorldGraphNode * WorldGraphNode::findNodeByName(const std::string &name)
{
    for (auto &node : this->children)
    {
        if (node->name == name)
        {
            return node;
        } else {
            return node->findNodeByName(name);
        }
    }

    return nullptr;
}

void WorldGraphNode::AttachParent(WorldGraphNode* parent)
{
    this->parent = parent;
    parent->children.push_back(this);
}

bool WorldGraphNode::HasParents(const std::string &name)
{
    if (this->parent == nullptr)
    {
        return false;
    }
    if (this->parent->name == name)
    {
        return true;
    }
    return this->parent->HasParents(name);
}

JsonSceneReader::JsonSceneReader(const std::string &file)
{
    root = new WorldGraphNode("root");
    root->isRoot = true;

    if (!std::filesystem::exists(file)){
        throw ResourceNotFoundException(("resource not found : " + file).c_str());
    }
    std::ifstream f(file);
    m_BasePath = std::filesystem::path(file).parent_path().string();
    if (!f.is_open())
    {
        return;
    }

    json j = json::parse(f);
    json geojsons = j["geometry"];
    json lights = j["lights"];

    if (j.contains("atomsphere")) // atomsphere has to be loaded firstly.  since geometry pbr shader need the functions provided by atomsphere model.
    {
        bool hasAtomsphere = j["atomsphere"];
        if (hasAtomsphere)
        {
            m_Atomsphere = new Atomsphere();
        }
    }

    ResolveHierarchy(geojsons, nullptr);
    ReadRoot(root);

    for (basic_json l : lights)
    {
        std::string lightpath = l;
        std::string path = fmt::format("{}/{}", m_BasePath, lightpath);

        json lightjson;
        std::ifstream lightfile(path);
        lightfile >> lightjson;
       
        Light *light = ReadLight(lightjson);
        m_Lights.push_back(light);
        lightfile.close();
    }

    if (j.contains("skybox"))
    {
        m_EnvBox= ReadEnvBox(j["skybox"]);
    }

    if (j.contains("settings"))
    {
        ResolveSceneSettings(j["settings"]);
    }

    if (j.contains("terrain"))
    {
        std::string tPath= j["terrain"];
        std::string path = fmt::format("{}/{}", m_BasePath, tPath); 
        json terrainjson;
        std::ifstream terrainfile(path);
        terrainfile >> terrainjson;
        m_Terrain = ReadTerrain(terrainjson);
    }
   
    f.close();
}

JsonSceneReader::~JsonSceneReader()
{
    // will not delete geos and lights.  ~Scene will free the pointers.
    delete root;
}

void JsonSceneReader::ReadRoot(WorldGraphNode * root)
{
    if (root != nullptr && (!root->isRoot))
    {
        EmptyTransform *g = ReadGeo(root);
        m_Geos.push_back(g);
    }
    for(auto &c : root->children)
    {
        ReadRoot(c);
    }
}

EmptyTransform * JsonSceneReader::ReadGeo(WorldGraphNode *root)
{
    json &geojson = root->geojson;

    GeometryFactory * gf = GeometryFactoryBuilder::GetFactory(geojson);
    EmptyTransform *g = gf->CreateGeometry(geojson); //GeometryFactory::CreateGeometry(geoType, subType, geoFilePath);

    delete gf;
    std::vector<std::string> shaders = ReadDefaultShaders(geojson);
    for (std::string s : shaders) {
       // g->AppendAdditionalShader(s);
    }
    //ReadGeoPostHandle(g, geojson);
    
    return g;
}

void JsonSceneReader::ReadGeoPostHandle(Geometry * g, json geojson)
{
    if (!geojson.contains("posthandle"))
    {
        return;
    }

    json operationJsons = geojson["posthandle"];
    for (basic_json operationJson : operationJsons)
    {
        // OP_NAME , OP_PARAM
        if (!operationJson.contains(Operation::OP_NAME))
        {
            continue;
        }

        std::unordered_map<std::string, std::any> operationDesc = GetOperationDesc(operationJson);
        OperationFactory * opFactory = OperationFactoryBuilder::GetOperationFactory(operationDesc);
        Operation * operation = opFactory->GetOperation();
        operation->assignTarget(g);
        operation->operate();
        delete operation;
        operation = NULL;
        delete opFactory;
        opFactory = NULL;
    }

}

std::unordered_map<std::string, std::any> JsonSceneReader::GetOperationDesc(json operationJson)
{
     std::string operationName = operationJson[Operation::OP_NAME];
     std::string operationType = operationJson[Operation::OP_TYPE];
     if (operationName.empty() || operationType.empty())
     {
        throw UnsupportedOperationException("operation name or operation type missing ! ");
     }
     std::unordered_map<std::string, std::any> result;
     result[Operation::OP_NAME] = operationName;
     result[Operation::OP_TYPE] = operationType;
     basic_json operationParamsJson = operationJson[Operation::OP_PARAM];

     std::unordered_map<std::string, std::string> operationParams;
     for (auto& [key, value]: operationParamsJson.items())
     {
        //operationParams["type"] = 
        std::string _v = value;
        operationParams[key] = _v;
     }
     result[Operation::OP_PARAM] = operationParams;
     return result;
}

Light * JsonSceneReader::ReadLight(json lightjson)
{
    std::string lighttype = lightjson["type"];

    LightFactory * lf = LightFactoryBuilder::GetFactory(lightjson);
    Light *l = lf->CreateLight(lightjson);
    delete(lf);lf = NULL;
    return l;
}

EmptyTransform * JsonSceneReader::ReadTerrain(json terrainjson)
{
    GeometryFactory *gf = GeometryFactoryBuilder::GetFactory(terrainjson);
    EmptyTransform *g = gf->CreateGeometry(terrainjson);
    delete gf;
    return g;
}

void JsonSceneReader::AddToScene(Scene &scene)
{

    for (int i = 0; i < m_Lights.size(); i++)
    {
        scene.AddLight(m_Lights[i]);
    }



    if (m_Atomsphere != nullptr)
    {
        scene.AddAtomsphere(m_Atomsphere);
        Global::ui->RegisterComponent(UIComponent::SUN, (void *)m_Atomsphere->GetSunDirection());
    }
    if (m_EnvBox!= nullptr)
    {
        scene.AddEnvBox(m_EnvBox);
    }
    for(int i = 0; i < m_Geos.size(); i++)
    {
        scene.AddGeoOrEmpty(m_Geos[i]);
    }
    if (m_Terrain !=nullptr)
    {
        scene.AddTerrain(static_cast<Terrain *>(m_Terrain));
    }


    scene.settings.showNormal = m_ShowNormal;

    Global::ui->RegisterComponent(UIComponent::LIGHTS, (void *)scene.GetLights());
    
}
std::string JsonSceneReader::ReadTexture(json textureJson, std::string key)
{
    try {
        std::string texturePath = textureJson.at(key);
        return texturePath;
    } catch (const json::exception& e)
    {
        return "";
    }
}
std::vector<std::string> JsonSceneReader::ReadDefaultShaders(json geoJson)
{
    std::vector<std::string> shaderVector;
    try {
        basic_json shaderJsons = geoJson.at("defaultShaders");
        
        for (basic_json shaderJson: shaderJsons)
        {
            std::string shader = shaderJson;
            shaderVector.push_back(shader);
        }

    } catch (const json::exception& e) 
    {
    }
    return shaderVector;
}

EnvBox *JsonSceneReader::ReadEnvBox(json skyboxJson)
{
    EnvBox *s = nullptr;
    if (skyboxJson.contains("hdri"))
    {
        std::string hdriFile = skyboxJson["hdri"];
        s = new HDRIBox(hdriFile);
        s->ActiveEnvironment();
    }
    if (skyboxJson.contains("cubemap"))
    {
        json cubemapJson = skyboxJson["cubemap"];
        std::string top = cubemapJson["top"];
        std::string bottom = cubemapJson["bottom"];
        std::string left = cubemapJson["left"];
        std::string right = cubemapJson["right"];
        std::string front = cubemapJson["front"];
        std::string back = cubemapJson["back"];

        CubeMapTexturePath t = {top, bottom, left, right, front, back};
        Skybox *ss = new Skybox();
        ss->ReadCubeMap(t);
        ss->ActiveEnvironment();
        s = ss;
    }
    return s;
}

void JsonSceneReader::ResolveHierarchy(json geojsons, WorldGraphNode * parent)
{
    //a, b, c, d
    // a c, b, 
    json geojsons2;
    for(auto &georef: geojsons)
    {
          //std::cout<<i<<std::endl;
        json geo;
        std::string path = m_BasePath;
        std::string geoBasePath = m_BasePath;
        if (georef.type() == json::value_t::string)
        {
            std::string geopath = georef;
            path = fmt::format("{}/{}", m_BasePath, geopath); //m_BasePath + "/" +i;
            geoBasePath = util::GetContainerFolder(path);
            std::ifstream geofile(path);
            geofile >> geo;
            geofile.close();
        } else {
            geo= georef;
        }

        geo["basepath"] = geoBasePath;

        if (geo.contains("name"))
        {
            std::string name = geo["name"];
            WorldGraphNode * node = new WorldGraphNode(name);
            node->geojson = geo;
            if (m_Dict.find(name) != m_Dict.end()) {
                char * errorMsg;
                sprintf(errorMsg, "same name found: %s", name.c_str());
                throw UnsupportedOperationException(errorMsg);
            }
            m_Dict[name] = node;
            if (parent != nullptr)
            {
                geo["parent"] = parent->name;
            }
            geojsons2.push_back(geo);
        } else {
            throw UnsupportedOperationException("geojson must have name");
        }
    }
    for (json &i: geojsons2)
    {
        ResolveNode(i, parent);
    }
}

void JsonSceneReader::ResolveNode(json geojson, WorldGraphNode * parent)
{
    if (!geojson.contains("name"))
    {
        return;
    }
    std::string name = geojson["name"];
    WorldGraphNode *node = m_Dict[name];
    node->geojson = geojson;
    if (geojson.contains("parent"))
    {
        std::string parent = geojson["parent"];
        if (m_Dict.find(parent) != m_Dict.end())
        {
            //node->parent = m_Dict[parent];
            WorldGraphNode *parentNode = m_Dict[parent];
            if (parentNode -> HasParents(name))
            {
                // cirular ref
                //node->AttachParent(root);
                throw UnsupportedOperationException("Circular Ref");
            } else {
                node->AttachParent(parentNode);
            }

        } else {
            node->AttachParent(root);
        }
    } else if (parent != nullptr) {
        node->AttachParent(parent);
    } else {
        node ->AttachParent(root);
    }

    if (geojson.contains("type") && geojson["type"] == "container")
    {
        if (!geojson.contains("geometry")) 
        {
            throw UnsupportedOperationException("container json must contains geometry node");
        }
        json geojsons = geojson["geometry"];
        ResolveHierarchy(geojsons, node);
    }
}

void JsonSceneReader::ResolveSceneSettings(json settingsjson)
{
    /**
    if (settingsjson.contains("csm"))
    {
        m_CSM = settingsjson["csm"];
    }*/

    if (settingsjson.contains("showNormal"))
    {
        m_ShowNormal = true;
    }
}