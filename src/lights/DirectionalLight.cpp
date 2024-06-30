#include "DirectionalLight.h"
#include "../shaders/ShaderType.h"
#include "../render/LightMaterial.h"
#include "../render/DirectionalLightMaterial.h"
#include <glm/glm.hpp>

DirectionalLight::DirectionalLight()
{
    //AttachShader(ShaderType::DIRECTIONAL_LIGHT_SHADER);
    InitMaterial();
    //m_LightMap = new DirectionalLightMap(Global::screen.width, Global::screen.height);
    //m_LightMap->EnableDebug();
    //m_LightMap = new DirectionalLightMapCSM(Global::screen.width, Global::screen.height, Global::camera);
    //m_LightMap->EnableDebug();
}
LightType DirectionalLight::GetLightType() const
{
    return DIRECTIONAL;
}

void DirectionalLight::InitMaterial()
{
    m_Mat = new DirectionalLightMaterial();
}
void DirectionalLight::GetImpact(LightImpact* impacts, int &size)
{
    //impacts.reserve(5);
    LightMaterial *lm = static_cast<LightMaterial *> (m_Mat);
    const glm::vec3 amb = lm->GetAmbient();
    const glm::vec3 diff= lm->GetDiffuse();
    const glm::vec3 spec = lm->GetSpecular();

    impacts[0] = {LightImpactKey::LIPT_DIRECTION, 3, {m_Direction.x, m_Direction.y, m_Direction.z}};
    impacts[1] = {LightImpactKey::LIPT_SPECUAR, 3, {spec.r, spec.g, spec.b}};
    impacts[2] = {LightImpactKey::LIPT_AMBIENT, 3, {amb.r, amb.g, amb.b}};
    impacts[3] = {LightImpactKey::LIPT_DIFFUSE, 3, {diff.r, diff.g, diff.b}};
    size = 4;
}

float DirectionalLight::GetFarPlane()
{
    LightMaterial *lm = static_cast<LightMaterial *> (m_Mat);
    return glm::length(lm->GetDiffuse()) * glm::length(lm->GetDiffuse());
}

std::string DirectionalLight::GetName()
{
    return "DirectionalLight " + std::to_string(GetIndex());
}

ShaderDirLight DirectionalLight::Serialize()
{
    LightMaterial *lm = static_cast<LightMaterial *> (m_Mat);
    ShaderDirLight sdl;
    sdl.intensity = lm->GetIntensity();
    sdl.ambient = {lm->GetAmbient(), 0.0f};
    sdl.castShadow = 1;
    sdl.diffuse = {lm->GetDiffuse(),.0f};
    sdl.direction = {m_Direction, 0};
    sdl.farPlane = GetFarPlane();
    sdl.index = GetIndex();
    sdl.specular = {lm->GetSpecular(),.0f};
    return sdl;
}