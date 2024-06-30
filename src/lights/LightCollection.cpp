#include "LightCollection.h"
void LightCollection::AddLight(Light * l)
{
    m_Lights.push_back(l);
    if (l->GetLightType() == LightType::POINT)
    {
        unsigned int pLightNum = (unsigned int)m_PointLights.size();
        std::string lightName = "Point Light " + std::to_string(pLightNum);
        m_PointLights.push_back(static_cast<PointLight *>(l));
        l->SetIndex(pLightNum);
    }

    if (l->GetLightType() == LightType::DIRECTIONAL)
    {
        unsigned int dLightNum = (unsigned int)m_DirectionalLights.size();
        std::string lightName = "Directional Light " + std::to_string(dLightNum);
        m_DirectionalLights.push_back(static_cast<DirectionalLight*>(l));
        l->SetIndex(dLightNum);
    }

    if (l->GetLightType() == LightType::SUN)
    {
        m_SunLight = static_cast<SunLight *>(l);
    }
    // ... other light types

}

void LightCollection::RemoveLight(Light * l)
{
    //m_Lights.erase(std::remove(m_Lights.begin(), m_Lights.end(), l), m_Lights.end()));
     m_Lights.erase(std::remove(m_Lights.begin(), m_Lights.end(), l), m_Lights.end());
    if (l->GetLightType() == LightType::POINT)
    {
        m_PointLights.erase(std::remove(m_PointLights.begin(), m_PointLights.end(), l), m_PointLights.end());
    }
    if (l->GetLightType() == LightType::DIRECTIONAL)
    {
        m_DirectionalLights.erase(std::remove(m_DirectionalLights.begin(), m_DirectionalLights.end(), l), m_DirectionalLights.end());
    }
}

LightCollection::~LightCollection()
{
    for (int i = 0; i < m_Lights.size(); i++)
    {
        delete m_Lights[i];
        m_Lights[i] = NULL;
    }
    m_Lights.clear();
    m_PointLights.clear();
    m_DirectionalLights.clear();
}


std::vector<Light*> LightCollection::GetLights() const
{
    return m_Lights;
}

std::vector<PointLight*> LightCollection::GetPointLights() const
{
    return m_PointLights;
}

std::vector<DirectionalLight*> LightCollection::GetDirectionalLights() const
{
    return m_DirectionalLights;
}

void LightCollection::SerializeLights()
{
    SerializeDirLights();
    SerializePointLights();
    SerializeSunLight();
}

void LightCollection::SerializePointLights()
{
    unsigned int lightsCount = (unsigned int)m_PointLights.size();
    ShaderPointLight *spl = (ShaderPointLight*)_malloca(lightsCount * sizeof(ShaderPointLight));
    for (int i = 0; i < lightsCount; i++)
    {
        spl[i] = m_PointLights[i]->Serialize();
    }
    LightUniformBufferBindings::FeedDynamicPointLights(lightsCount > 0 ? spl : nullptr, lightsCount);
    
}

void LightCollection::SerializeDirLights()
{
    const int lightsCount = (int)m_DirectionalLights.size();
    ShaderDirLight *sdl = (ShaderDirLight*)_malloca(lightsCount * sizeof(ShaderDirLight));
    for (int i = 0; i < m_DirectionalLights.size(); i++)
    {
        sdl[i] = m_DirectionalLights[i]->Serialize();
    }
    LightUniformBufferBindings::FeedDynamicDirLights(lightsCount > 0 ? sdl : nullptr, lightsCount);
}

void LightCollection::SerializeSunLight()
{
    // with a m_SunLight
    // light struct with a enable = 1;
    // otherwise light struct with a enable = 0;
    if (m_SunLight == nullptr)
    {
        ShaderSunLight ssl;
        LightUniformBufferBindings::FeedSunLight(&ssl);
    } else {
        ShaderSunLight ssl = m_SunLight->Serialize();
        LightUniformBufferBindings::FeedSunLight(&ssl);
    }
    
}

bool LightCollection::HasDirectionalLights() const
{
    return m_DirectionalLights.size() > 0;
}

bool LightCollection::HasPointLights() const
{
    return m_PointLights.size() > 0;
}
bool LightCollection::HasSunLight() const
{
    return m_SunLight != nullptr;
}