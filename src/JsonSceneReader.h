#pragma once
#include <vector>
#include "global/GlobalData.h"
#include "nlohmann/json.hpp"
#include "geometry/Geometry.h"
#include "skybox/EnvBox.h"
#include "skybox/Skybox.h"
#include "skybox/HDRIBox.h"
#include "textures/CubeMapTexture.h"
#include "lights/Light.h"
#include "terrain/Terrain.h"
#include "atomsphere/Atomsphere.h"
#include "water/Sea.h"
#include "Scene.h"
#include <unordered_map>
#include <vector>
#include <queue>
#include <any>
#include <sstream>
#include "Logger.h"
#include "water/Sea.h"

using namespace nlohmann;
class WorldGraphNode
{
public:
    WorldGraphNode(const std::string &name);
    ~WorldGraphNode();
    WorldGraphNode *parent = nullptr;
    std::vector<WorldGraphNode*> children;
    std::string name;
    json geojson;
    bool isRoot = false;
    WorldGraphNode * findNodeByName(const std::string &name);
    void AttachParent(WorldGraphNode* parent);
    bool HasParents(const std::string &name);
   
};

class JsonSceneReader
{
private:
    std::string m_BasePath;
    bool m_ShowNormal = false;
    bool m_CamNav = false;
    WorldGraphNode *root;
    std::vector<EmptyTransform *> m_Geos;
    std::vector<Light *> m_Lights;
    EnvBox *m_EnvBox = nullptr;
    EmptyTransform *m_Terrain = nullptr;
    Atomsphere *m_Atomsphere = nullptr;
    Sea* m_Sea = nullptr;
    void ResolveHierarchy(json g, WorldGraphNode *);
    void ResolveNode(json geojson, WorldGraphNode*);
    void ResolveSceneSettings(json settingsjson);
    std::queue<std::string> m_Order;
    std::unordered_map<std::string, WorldGraphNode*> m_Dict;
public:
    JsonSceneReader(const std::string &file);
    void ReadRoot(WorldGraphNode*);
    EmptyTransform* ReadGeo(WorldGraphNode*);
    void ReadGeoPostHandle(Geometry * g, json geojson);
    Light* ReadLight(json lightjson);
    EmptyTransform* ReadTerrain(json terrainjson);
    EnvBox *ReadEnvBox(json skyboxJson);
    ~JsonSceneReader();
    void AddToScene(Scene &scene);
    std::string ReadTexture(json json, std::string key);
    std::vector<std::string> ReadDefaultShaders(json geojson);
    std::unordered_map<std::string, std::any> GetOperationDesc(json);
};