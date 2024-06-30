#include "AdditionalShader.h"

AdditionalShader::AdditionalShader()
{
    m_TargetGeo = nullptr;
}
AdditionalShader::~AdditionalShader()
{
}

void AdditionalShader::PreHandle()
{

}

void AdditionalShader::PostHandle()
{

}

void AdditionalShader::AddOperation(void (*x)(void))
{
    m_Ops.push_back(x);
}

void AdditionalShader::AddTargetGeo(Geometry *geo)
{
    m_TargetGeo = geo;
}

void AdditionalShader::Render()
{
   // m_TargetGeo -> RenderDrawCall();
}

void AdditionalShader::OnUpdateMVP(glm::mat4 model, glm::mat4 view, glm::mat4 projection, glm::vec3 camPos)
{
    
}