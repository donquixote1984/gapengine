#pragma once
#include <string>
#include <vector>
#include "lights/LightCollection.h"
#include "geometry/Empty.h"
#include "geometry/Geometry.h"
#include "Camera.h"
#include "Ground.h"
#include "TestGeometry.h"
#include "skybox/EnvBox.h"
#include "render/NormalRenderer.h"
#include "render/EmptyRenderer.h"
#include "render/lightmap/LightMapRenderer.h"
#include "ui/UISettings.h"
#include "exceptions/JumpException.h"
#include "render/lightmap/LightMap.h"
#include "render/lightmap/PointLightMap.h"
#include "terrain/Terrain.h"
#include "atomsphere/Atomsphere.h"
#include "water/Sea.h"
#include "render/passes/RenderPassLinkList.h"
#include <typeinfo>




class TestGeometry;
struct SceneSettings
{
    bool showNormal = false;

};
class Scene
{
private:
    //Light * m_Light;
    std::vector<EmptyTransform*> m_Empties;
    std::vector<ShadedEmptyTransform*> m_Geos;
    LightCollection *m_Lights = nullptr;
    std::vector<PointLight *> m_PointLights;
    std::vector<DirectionalLight *> m_DirectionalLights;
    Camera *m_Camera = nullptr;
    Ground *m_Ground = nullptr;
    Terrain *m_Terrain = nullptr;
    Sea* m_Sea = nullptr;
    Atomsphere *m_Atomsphere = nullptr;
    TestGeometry *m_TestGeometry = nullptr;
    void UpdateMVP();
   
    EnvBox *m_EnvBox= nullptr;

    EmptyRenderer m_Er;
    LightMapRenderer m_LMR;
    NormalRenderer m_Nr;
    RenderPassLinkList *m_RenderPasses;
    
    void InitRenderPasses();
    unsigned int m_Frame = 0;
    float m_LastFrameTime = 0;
    bool m_CamNav = false;
public:
    SceneSettings settings;
    Scene();
    ~Scene();
    Scene(const Scene &) = delete;
    void Bind();
    void AddLight(Light *light);
    void RemoveLight(Light * light);
    void AddGeoOrEmpty(EmptyTransform *geo);
    
    void AddEnvBox(EnvBox *s);
    void AddGround(Ground *ground);
    void AddCamera(Camera *camera);
    //std::vector<Geometry *> GetGeometries();
    const LightCollection * GetLights();
    void OnUpdate(GLFWwindow *window);
    std::vector<ShadedEmptyTransform*> & GetGeos();
    EnvBox *GetEnvBox() const;
    bool UseCSM();
    void AddTerrain(Terrain *);
    void AddSea(Sea*);
    void AddAtomsphere(Atomsphere * atomsphere);
    const LightMapRenderer &GetLightMapRenderer() const;
    void AppendPreRenderPass(RenderPass *pass);
    void AppendPostRenderPass(RenderPass *pass);
    void RenderEnv(RenderContext &rc);
    void SetCamNav(bool);
    RenderContext InverseCamera(float height);
    RenderContext ReverseCamera(float height);
    float GetEnvExposure();
};