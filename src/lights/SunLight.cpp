#include "SunLight.h"
#include "../global/Sky.h"

ShaderSunLight SunLight::Serialize()
{
    glm::vec3 depth = Global::atomsphere->GetDepth();
    ShaderSunLight ssl;
    ssl.castShadow = 1;
    ssl.diffuse = glm::vec4(1);
    ssl.direction = {m_Direction, 0};
    ssl.farPlane = 0; 
    ssl.intensity = 0;
    ssl.enable = 1;
    ssl.depth = glm::vec4(depth, 0);
    return ssl;
}

LightType SunLight::GetLightType() const
{
    return LightType::SUN;
}