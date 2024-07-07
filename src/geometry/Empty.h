#pragma once
#include <string>
#include <functional>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include "../script/GeometryScript.h"
#include "../render/Material.h"
#include "../shaders/Shader.h"
#include "../UniformBuffer.h"

class Scene;
class RenderPassLinkList;

class EmptyTransform
{
protected:
    glm::mat4 m_Model =  glm::mat4(1.0f);
    glm::vec3 m_Position;
    std::string m_Name;
    glm::vec3 m_Scale=glm::vec3(1.0);
    glm::vec3 m_InitScale = glm::vec3(1.0);
    //glm::vec3 m_Rotate = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::quat m_Rotate = glm::angleAxis(0.0f, glm::vec3(0, 1, 0));
    EmptyTransform *m_Parent = nullptr;
    std::vector<EmptyTransform *> children;
    bool m_Visible = true;
    GeometryScript * m_Script = NULL;
    Scene * m_Scene = nullptr;
    glm::mat4 m_TransMat;
    RenderPassLinkList *m_RenderPasses = nullptr; // have to be ptr for circluar reference
   public:
    virtual ~EmptyTransform();
    EmptyTransform * GetParent();
    void SetVisible(bool v);
    bool IsVisible();
    std::string GetName();
    void AttachParent(EmptyTransform *parent);
    void SetName(std::string name);
    void Scale(glm::vec3);
    void Translate(glm::vec3);
    void Rotate(float, glm::vec3);
    void Rotate(glm::quat rot);
    void SetPosition(glm::vec3);
    virtual void RenderDrawCall();
    virtual void RenderDrawCall(Shader* shader);
    glm::mat4 & GetModelMat();
    void SetInitScale(glm::vec3 scale);
    virtual void AttachScript(const std::string &script);
    virtual void OnUpdateScript(GLFWwindow *window);
    virtual bool IsEmpty();
    virtual bool IsGeometry();
    virtual void SetScene(Scene *s);
    const Scene* GetScene() const;
    virtual void ReadyToScene();
    virtual void InitPasses();
    RenderPassLinkList* GetRenderPass();
};

class ShadedEmptyTransform: public EmptyTransform
{
protected:
    std::vector<Material*> m_Mats;
    ShaderUniformsCache m_ShaderUniformsCache;

public:    
    ~ShadedEmptyTransform();
    virtual void AddMaterial(Material *m);
    virtual void ClearMaterial();
    virtual bool IsInstancing();
    virtual void PreRender();
    std::vector<Material* > GetMaterial() const;
    void ReadyToScene() override;
    bool IsEmpty() override;
    bool IsGeometry() override;
    void HandleMaterials(std::function<void(Material*)> ops);
    ShaderUniformsCache& GetUniforms();
};