#pragma once
#include "../shaders/Shader.h"
#include "../shaders/ShaderSnippet.h"
#include <glm/glm.hpp>
#include <map>
#include "../ui/UISettings.h"
#include "MaterialAttrib.h"

class Material
{
protected:
    Shader *m_IntrinsicShader = nullptr;
public:
    Shader* GetShader() const;
    virtual ~Material();
    virtual void FlushToShader() = 0;
    //virtual void MVPToShader(glm::mat4 model, glm::mat4 view, glm::mat4 projection, glm::vec3 camPos) const = 0;
    virtual void InitAttribs() = 0;
    void AddShaderArgs(std::string key, std::string value);
    void AddShaderArgs(std::map<std::string, std::string> map);
    void AddShaderSnippet(ShaderSnippet ss);
    void AddShaderPartial(std::string key, std::string content);
    void FlushToGlobalSettings();
    void BindMaterialShaderAttrib(MaterialAttrib &mAttrib, unsigned int index) const;
};