#include <format>
#include "Scene.h"
#include "FrameBufferDebugger.h"


Scene::Scene()
{
   //m_TestGeometry = new TestGeometry();
   m_Lights = new LightCollection();
   m_LMR.SetSceneLights(m_Lights);
   InitRenderPasses();
}
Scene::~Scene()
{
    for (int i = 0; i < m_Geos.size(); i++)
    {
        delete m_Geos[i];
        m_Geos[i] = NULL;
    }
    m_Geos.clear();

    delete(m_Ground);
    m_Ground = NULL;
    delete(m_Camera);
    m_Camera = NULL;
    delete(m_EnvBox);
    m_EnvBox= NULL;
    delete m_Lights;
    m_Lights = NULL;
    delete m_Terrain;
    m_Terrain = NULL;
    delete m_Atomsphere;
    m_Atomsphere = NULL;
    delete m_Sea;
    m_Sea = NULL;
}
void Scene::InitRenderPasses()
{
    m_RenderPasses = new RenderPassLinkList();
    m_RenderPasses->AppendRenderPass(RenderPassLinkList::CreateSceneRenderPass(RenderPassType::SHADOW));
    m_RenderPasses->AppendRenderPass(RenderPassLinkList::CreateSceneRenderPass(RenderPassType::SCREEN));
    //m_RenderPasses->AppendRenderPass(RenderPassLinkList::CreateSceneRenderPass(RenderPassType::POSTPROCESS));
    m_RenderPasses->AppendRenderPass(RenderPassLinkList::CreateSceneRenderPass(RenderPassType::FRAME_BUFFER_DEBUG));
}

void Scene::AddLight(Light *light)
{
    m_Lights->AddLight(light);
}

void Scene::RemoveLight(Light * light)
{
    // update each light index;
    m_Lights->RemoveLight(light);
}

void Scene::AddGeoOrEmpty(EmptyTransform *geo)
{
    if (geo->IsEmpty()) 
    {
        m_Empties.emplace_back(geo);
    } else 
    {
        m_Geos.emplace_back(static_cast<ShadedEmptyTransform *>(geo));
    }
    geo->SetScene(this);


}


void Scene::AddCamera(Camera *camera)
{
    m_Camera = camera;
}
void Scene::OnUpdate(GLFWwindow *window)
{
    m_Frame += 1;

    float currentFrameTime = static_cast<float>(glfwGetTime());
    float deltaTime = currentFrameTime - m_LastFrameTime;
    m_LastFrameTime = currentFrameTime;

    m_Camera->OnUpdate(window);
    m_Er.OnRenderEmpties(m_Empties, window);

    RenderContext rc = {m_Camera->GetProjection(), m_Camera->GetView(), m_Camera->GetPosition(), m_Camera, m_Frame, deltaTime};
    glm::mat4 mats[2] = {rc.projection, rc.view};
    MatricesUniformBufferBindings::FeedMatrices(mats, 2);
    //SettingsUniformBufferBindings::FeedSettings(&settings::UISettings::GetSettings()->Serialize());
    settings::UISettings::GetSettings()->FlushToShader();
    m_Lights->SerializeLights();
    m_LMR.OnRenderShadowSpace(window);

    for (auto geo: m_Geos)
    {
        geo->OnUpdateScript(window);
    }

  
    try {
        m_RenderPasses->Render(rc);
        //m_Gr.SetRC(rc);
        //m_Gr.SetSceneLights(m_Lights);
        //m_Gr.OnRenderGeos(m_Geos, window);
        if (settings.showNormal)
        {
            m_Nr.SetRC(rc);
            m_Nr.OnRenderGeo(m_Terrain, window);
        }

        //FrameBufferDebugger::get()->Debug();
        
    } catch (JumpException e)
    {}
    //m_TestGeometry->OnRenderUpdate(rc);
}

const LightCollection * Scene::GetLights()
{
    return m_Lights;
}
void Scene::AddGround(Ground * ground)
{
    m_Ground = ground;
}

void Scene::AddEnvBox(EnvBox *s)
{
    m_EnvBox = s;
}

EnvBox *Scene::GetEnvBox() const
{
    return m_EnvBox;
}
void Scene::AddTerrain(Terrain *terrain)
{
    if (m_Terrain == nullptr) {
        m_Terrain = terrain;
        m_Geos.push_back(m_Terrain);
    }
    terrain->SetScene(this);
}
void Scene::AddSea(Sea* sea)
{
    m_Sea = sea;
}
void Scene::SetCamNav(bool nav)
{
}
void Scene::AddAtomsphere(Atomsphere * atomsphere)
{
    m_Atomsphere = atomsphere;
    m_Lights->AddLight(m_Atomsphere->GetSunLight());
    m_LMR.EnableSunLight(m_Atomsphere->GetSunLight());
}

const LightMapRenderer &Scene::GetLightMapRenderer() const
{
    return m_LMR;
}

void Scene::AppendPreRenderPass(RenderPass * pass)
{
    m_RenderPasses->PrependToFirstType(RenderPassType::SCREEN, pass);
}
void Scene::AppendPostRenderPass(RenderPass * pass)
{
    m_RenderPasses->AppendToFirstType(RenderPassType::SCREEN, pass);
}

std::vector<ShadedEmptyTransform*> & Scene::GetGeos()
{
    return m_Geos;
}

void Scene::RenderEnv(RenderContext & rc)
{
    if (m_EnvBox!= nullptr)
    {
        m_EnvBox-> OnUpdateMVP(rc);
    } 
    if (m_Atomsphere != nullptr)
    {
        m_Atomsphere->OnRender();
    }

    if (m_Sea != nullptr)
    {
        m_Sea->OnRender();
    }
    m_Ground->OnUpdateRender(rc);
}

RenderContext Scene::InverseCamera(float height)
{
    m_Camera->InverseY(height);
    RenderContext rc = {m_Camera->GetProjection(), m_Camera->GetView(), m_Camera->GetPosition(), m_Camera};
    glm::mat4 mats[2] = {rc.projection, rc.view};
    MatricesUniformBufferBindings::FeedMatrices(mats, 2);
    return rc;
}

RenderContext Scene::ReverseCamera(float height)
{
    m_Camera->ReverseY(height);
    RenderContext rc = {m_Camera->GetProjection(), m_Camera->GetView(), m_Camera->GetPosition(), m_Camera};
    glm::mat4 mats[2] = {rc.projection, rc.view};
    MatricesUniformBufferBindings::FeedMatrices(mats, 2);
    return rc;
}

float Scene::GetEnvExposure()
{
    return m_EnvBox->GetExposure();
}