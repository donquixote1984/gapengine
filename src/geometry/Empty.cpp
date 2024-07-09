#include "Empty.h"
#include "../Scene.h"

EmptyTransform * EmptyTransform::GetParent()
{
    return m_Parent;
}

EmptyTransform::~EmptyTransform()
{
    delete m_RenderPasses;
    m_RenderPasses = nullptr;
}
void EmptyTransform::SetVisible(bool v)
{
    m_Visible = v;
}
bool EmptyTransform::IsVisible()
{
    return m_Visible;
}

void EmptyTransform::SetName(std::string name)
{
    m_Name = name;
}

std::string EmptyTransform::GetName()
{
    return m_Name;
}


void EmptyTransform::Scale(glm::vec3 scale)
{
    m_Scale = scale;
}

void EmptyTransform::Translate(glm::vec3 trans)
{
     m_Position = trans;
}

void EmptyTransform::Rotate(float angle, glm::vec3 axis)
{
    glm::quat q = glm::angleAxis(glm::radians(angle), axis);
    m_Rotate = q * m_Rotate;
}
void EmptyTransform::Rotate(glm::quat rot)
{
    m_Rotate = rot * m_Rotate;
}

void EmptyTransform::SetPosition(glm::vec3 position)
{
    m_Position = position;
}

void EmptyTransform::AttachParent(EmptyTransform * parent)
{
    if (parent != nullptr)
    {
        this->m_Parent = parent;
       parent->children.push_back(this);
    }
}

glm::mat4 & EmptyTransform::GetModelMat()
{
    /*
    glm::mat4 &model;
    glm::mat4 &view;
    glm::mat4 &projection;
    glm::vec3 &camPos;
    */
    m_TransMat = glm::mat4(1.0);
    if (m_Parent != nullptr)
    {
        m_TransMat = m_Parent->m_TransMat * m_TransMat;
    }
   
    m_TransMat = glm::translate(m_TransMat, m_Position); 
    m_TransMat = glm::mat4_cast(m_Rotate) * m_TransMat;

    //float angle = glm::length(m_Rotate) - 1;
    //glm::vec3 axis = glm::normalize(m_Rotate);

    
    m_TransMat = glm::scale(m_TransMat, m_InitScale * m_Scale);
    
    //return {model, };

    return m_TransMat;
}
void EmptyTransform::SetInitScale(glm::vec3 scale)
{
    m_InitScale = scale;
}

void EmptyTransform::AttachScript(const std::string &script)
{
   ScriptContext sc;
   sc.position = &this->m_Position;
   sc.rotation = &this->m_Rotate;
   sc.scale    = &this->m_Scale;
   m_Script = new GeometryScript(script);
   m_Script->ResetContext(sc);
}

void EmptyTransform::OnUpdateScript(GLFWwindow *window)
{
    if (m_Script != nullptr)
    {
        m_Script->OnFrame(window);
        //ScriptCallback();
    }
    //float currentFrame = static_cast<float>(glfwGetTime());
    //Rotate(glm::vec3(0.0f, 1.0f, 0.00f), currentFrame/10.0);
}

bool EmptyTransform::IsGeometry()
{
    return false;
}
bool EmptyTransform::IsEmpty()
{
    return true;
}
void EmptyTransform::RenderDrawCall()
{}

void EmptyTransform::RenderDrawCall(Shader * shader)
{}

void EmptyTransform::SetScene(Scene *s)
{
    m_Scene = s;
    this->ReadyToScene();
}
const Scene* EmptyTransform::GetScene() const
{
    return m_Scene;
}

void EmptyTransform::ReadyToScene()
{

}
void EmptyTransform::InitPasses()
{}

RenderPassLinkList *EmptyTransform::GetRenderPass()
{
    return m_RenderPasses;
}

void ShadedEmptyTransform::HandleMaterials(std::function<void(Material*)> opts)
{
    std::for_each(m_Mats.begin(), m_Mats.end(), opts);
}

void ShadedEmptyTransform:: AddMaterial(Material *m)
{
    m_Mats.push_back(m);
}

void ShadedEmptyTransform::ClearMaterial()
{
    for (int i = 0; i < m_Mats.size(); i++)
    {
        delete m_Mats[i];
    }

    m_Mats.clear();
}

ShaderUniformsCache& ShadedEmptyTransform::GetUniforms()
{
    return m_ShaderUniformsCache;
}
ShadedEmptyTransform::~ShadedEmptyTransform()
{
    for (Material* m : m_Mats)
    {
        if (m != nullptr)
        {
            delete m;
        }
    }
}

std::vector<Material*> ShadedEmptyTransform::GetMaterial() const
{
    return m_Mats;
}
bool ShadedEmptyTransform::IsEmpty()
{
    return false;
}

bool ShadedEmptyTransform::IsInstancing()
{
    return false;
}
void ShadedEmptyTransform::PreRender()
{}

void ShadedEmptyTransform::ReadyToScene()
{
    for (auto mat : m_Mats)
    {
        Shader* shader = mat->GetShader();
        shader->Ready();
        shader->BindToUniformBuffer(LightUniformBufferBindings::LightSlot);
        shader->BindToUniformBuffer(MatricesUniformBufferBindings::MatricesSlot);
        shader->BindToUniformBuffer(MatricesUniformBufferBindings::CSMSlot);
        shader->BindToUniformBuffer(MatricesUniformBufferBindings::PSMSlot);
        shader->BindToUniformBuffer(MatricesUniformBufferBindings::SUNSlot);
        shader->BindToUniformBuffer(SettingsUniformBufferBindings::SettingsSlot);
        shader->BindToUniformBuffer(BoneUniformBufferBindings::BoneSlot);
    }
}


bool ShadedEmptyTransform::IsGeometry()
{
    return false;
}