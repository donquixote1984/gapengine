#include <glm/glm.hpp>
#include "OutlineShader.h"
#include "DefaultShaderOperationUtils.h"

OutlineShader::OutlineShader()
{
    //AddOperation(&DefaultShaderOperationUtils::EnableStencil);
    //AddOperation(&DefaultShaderOperationUtils::SetCurrentObjAsStencil);
    // std::unique_ptr<Shader> s(new Shader("defaultshader/shaders/outline.shader"));
    m_OutlineShader = new Shader("defaultshader/shaders/outline.shader");
}
OutlineShader::~OutlineShader()
{
    delete(m_OutlineShader);
}
void OutlineShader::Render()
{
    DefaultShaderOperationUtils::EnableStencil();
    DefaultShaderOperationUtils::ReplaceStencilWhenAllPass();
    DefaultShaderOperationUtils::SetCurrentObjAsStencil();
  //  m_TargetGeo->RenderDrawCall();

    DefaultShaderOperationUtils::DisableStencil();
    m_OutlineShader->Bind();
    DefaultShaderOperationUtils::OutputIfNotStencil();
  //  m_TargetGeo->RenderDrawCall();
    DefaultShaderOperationUtils::ClearStencil();
}

void OutlineShader::OnUpdateMVP(glm::mat4 model, glm::mat4 view, glm::mat4 projection, glm::vec3 camPos)
{
    m_OutlineShader->Bind();
    model = glm::scale(model, glm::vec3(1.1, 1.1, 1.1));
    m_OutlineShader->setUniform4m("projection", projection);
    m_OutlineShader->setUniform4m("view", view);
    m_OutlineShader->setUniform4m("model", model);
    m_OutlineShader->setUniform3f("viewPos", camPos.x, camPos.y, camPos.z);
}