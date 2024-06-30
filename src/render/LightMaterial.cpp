#include "LightMaterial.h"
#include "../shaders/GeometryShader.h"
LightMaterial::LightMaterial()
{
    InitAttibs();
}

void LightMaterial::InitAttibs()
{
    ambient.uniform = "ambient";
    specular.uniform = "specular";
    diffuse.uniform = "diffuse";
    constant.uniform = "constant";
    linear.uniform = "linear";
    quadratic.uniform = "quadratic";
}
glm::vec3 LightMaterial::GetAmbient()
{
    return ambient.color; //ValueToVec3(ambient);
}
glm::vec3 LightMaterial::GetSpecular()
{
    return specular.color; //ValueToVec3(specular);
}
glm::vec3 LightMaterial::GetDiffuse()
{
    return diffuse.color;
}
float LightMaterial::GetIntensity()
{
    return intensity.constant;
}
float LightMaterial::GetLinear()
{
    return linear.constant;
}
float LightMaterial::GetConstant()
{
    return constant.constant;
}
float LightMaterial::GetQuadratic()
{
    return quadratic.constant;
}

void LightMaterial::FlushToShader() 
{
    this->m_IntrinsicShader->Bind();
    this->m_IntrinsicShader->setUniform3f("light.diffuse", diffuse.color.x, diffuse.color.y, diffuse.color.z);
}
/*
void  LightMaterial::MVPToShader(glm::mat4 model, glm::mat4 view, glm::mat4 projection, glm::vec3 camPos) const 
{
    this->m_IntrinsicShader->Bind();
    this->m_IntrinsicShader->setUniform4m("model", model);
    this->m_IntrinsicShader->setUniform4m("projection", projection);
    this->m_IntrinsicShader->setUniform4m("view", view);
}
*/