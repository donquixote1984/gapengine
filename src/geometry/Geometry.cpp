#include "glad/glad.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <fmt/format.h>
#include "Geometry.h"

//#include "../defaultshader/AdditionalShader.h"
//#include "../defaultshader/AdditionalShaderFactory.h"
#include "../exceptions/GeometryInitializeException.h"
#include "../render/DefaultMaterial.h"
#include "../render/passes/RenderPassLinkList.h"



Geometry::Geometry(GeometryType geoType):m_Type(geoType)
{
    this->InitGeoData();
    this->InitMaterial();
}
void Geometry::InitGeoData()
{}

Geometry::~Geometry()
{
    delete m_Dp;
    delete m_Script;
    delete m_RenderPasses;
    m_Dp = nullptr;
    m_Script = nullptr;
    m_RenderPasses = nullptr;
}

void Geometry::InitGeometry(const std::vector<Vertex> &vertices, const std::vector<unsigned int> &indices)
{
}
void Geometry::InitMaterial()
{
   // m_Mat = new DefaultMaterial();
}

void RenderToScene()
{
    /*
    m_IntrinsicShader->Bind();
    m_VAO.Bind();
    if (m_AdditionalShaders.size() == 0)
        RenderDrawCall();
    else
        RenderWithDefaultShaders();
        */
}

void Geometry::ReadyToScene()
{
    if (m_GeoData->HasBone())
    {
        HandleMaterials([](Material* mat) {
            mat->AddShaderSnippet(ShaderSnippet::AnimationSnippet());
            });
    }
    ShadedEmptyTransform::ReadyToScene();
   
}

void Geometry::RenderDrawCall()
{
    /*
    if (m_HasVariant) {
        int x = util::RandInRange(0, m_DataVariantNum-1);
        //m_GeoData[0].Draw();
        m_Dp->Draw();
        
    } else {
        //m_GeoData[0].Draw();
        m_Dp->Draw();
    }
    */

    int clusterNum = 0;
    auto rawVertices = m_Dp->GetRawVertices(clusterNum);

    for (int i = 0; i < clusterNum; i++)
    {
        if (i <= m_Mats.size() - 1) {
            static_cast<BasicMaterial*>(m_Mats[i])->FlushToShader(m_ShaderUniformsCache);
        }
        rawVertices[i].Draw(DisplayMode::MESH);
    }

    m_ShaderUniformsCache.Clear();

}

void Geometry::RenderDrawCall(Shader* shader)
{
    int clusterNum = 0;
    auto rawVertices = m_Dp->GetRawVertices(clusterNum);
    shader->Bind();
    for (int i = 0; i < clusterNum; i++)
    {
        rawVertices[i].Draw(DisplayMode::MESH);
    }
}
void Geometry::OnUpdateRender(GLFWwindow *window)
{
    RenderToScene();
}
bool Geometry::IsGeometry()
{
    return true;
}

void Geometry::SetDisplayMode(DisplayMode mode)
{
    m_Dp->SetDisplayMode(mode);
}
void Geometry::SetScale(glm::vec3 scale)
{
    m_Scale = scale;
}

DisplayMode Geometry::GetDisplayMode() const
{
    return m_Dp->GetDisplayMode();
}

void Geometry::InitGeometryData(float * vertices, unsigned int count, std::initializer_list<int> layouts)
{
    m_GeoData =  GeometryDataFactory::CreateGeometryData(vertices, count, layouts);
}
void Geometry::InitGeometryData(GeometryData * data, uint8_t variant)
{
    m_DataVariantNum = variant;
    m_GeoData = data; // just 1 variant; 

    MoveEmbedTexturesToMaterial();
}
void Geometry::InitGeometryData(std::shared_ptr<float[]> data, unsigned int count,  std::initializer_list<int> layouts)
{
    //m_DataVariantNum = variant;
    m_GeoData = GeometryDataFactory::CreateGeometryData(data, count, layouts);; // just 1 variant; 
}

void Geometry::InitGeometryVariant(GeometryData * data, uint8_t variant)
{
    if (variant > constants::MAX_GEO_VARIANT)
    {
        throw GeometryInitializeException("geo variant number exceed MAX_GEO_VARIANT");
    }
    m_HasVariant = true;
    m_GeoData = data;
    m_DataVariantNum = variant;
    MoveEmbedTexturesToMaterial();
}

void Geometry::AddPostProcessing(DataPostProcessing * dpp)
{
    if (dpp->GetType() == PostProcessingType::SCATTER) 
    {
        //SwitchMaterial(SharedMaterial::GetSharedPointMaterial());
    }
    static_cast<GeometryDataProcessor*>(m_Dp)->AddPostProcessing(dpp);
}

void Geometry::ProcessData()
{
    m_Dp->Process(this);
}
void Geometry::AttachMeta(GeometryMeta meta)
{
    m_Meta = meta;
}
void Geometry::ResolveMeta()
{}

GeometryMeta & Geometry::GetMeta()
{
    return m_Meta;
}

int Geometry::GetVerticesCount()
{
    std::vector<Mesh> meshes = m_Dp->GetProcessedData();
    int v = 0;
    for (auto & m : meshes)
    {
        v += m.GetVerticesCount();
    }
    return v;
}

DataProcessor * Geometry::GetDataProcessor()
{
    return m_Dp;
}

bool Geometry::IsInstancing()
{
    return false;
}

void Geometry::InitProcessor()
{
    m_Dp = new GeometryDataProcessor();
}
void Geometry::ScriptCallback()
{
    auto ctx = m_Script->GetContext();
    SetPosition(
    *(ctx->position)
    );

    SetScale(
        *(ctx->scale)
    );

    Rotate(
        *(ctx->rotation)
    );
}

void Geometry::AttachScript(const std::string &script)
{
   ScriptContext sc;
   sc.position = &this->m_Position;
   sc.rotation = &this->m_Rotate;
   sc.scale    = &this->m_Scale;
   m_Script = new GeometryScript(script);
   m_Script->ResetContext(sc);
}
bool Geometry::IsEmpty()
{
    return false;
}

GeometryData *Geometry::GetGeoData()
{
    return m_GeoData;
}

void Geometry::InitPasses()
{
    m_RenderPasses = new RenderPassLinkList();
    m_RenderPasses->AppendRenderPass(RenderPassLinkList::CreateRenderPass(RenderPassType::GEOMETRY, this));
}

void Geometry::SetShadowPass(bool shadowPass)
{
    m_ShadowPass = shadowPass;
}
void Geometry::PreRender(){}

bool Geometry::HasTess()
{
    return false;
}
void Geometry::MoveEmbedTexturesToMaterial()
{
    //TODO currently only use single material per geometry(which may contains mulitple mesh),  will refactor to multiple material.
    //TODO 
    auto meshes = m_GeoData->GetMeshes();

    for (int i = 0; i < meshes.size(); i++)
    {
        for (auto texture : meshes[i].GetTextures())
        {
            if (texture.embeded && m_Mats.size() == meshes.size())
            {
                BasicMaterial* m = static_cast<BasicMaterial*>(m_Mats[i]);
                m->UpdateTextureFromMemory(texture.type, texture.data);
            }
        }
    }
}

bool Geometry::IsPlaying()
{
    return m_Playing;
}

void Geometry::SetPlaying(bool playing)
{ 
    m_Playing = playing;
}

Animator * Geometry::GetAnimator()
{
    return &m_Animator;
}

void Geometry::PlayAnimation(unsigned int animationIndex)
{
    //m_Animator.SetCurrentAnimation(m_GeoData)
    m_Animator.SetCurrentSceneAnimation(m_GeoData->GetSceneAnimation(animationIndex));

}

void Geometry::FeedAnimations(std::vector<SceneAnimation *> sceneAnimations)
{
    if (sceneAnimations.size() > 0)
    {
        m_GeoData->FeedSceneAnimation(sceneAnimations);
    }
}