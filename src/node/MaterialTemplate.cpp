#include "MaterialTemplate.h"

/***
 * 
 *     
     *     float shininess=32.0f;
    // point light
    float linear=0.09;
    float constant=1.0f;
    float quadratic=0.032f;
    */
DefaultGeometryMaterialTemplate::DefaultGeometryMaterialTemplate()
{
    //m_Channels= {"ambient", "diffuse", "specular", "shininess"};
    m_Channels.push_back({"ambient",   MaterialValueNodeEnum::AMBIENT, MaterialAttribType::COLOR, glm::vec3(1.0f)});
    m_Channels.push_back({"diffuse",   MaterialValueNodeEnum::DIFFUSE, MaterialAttribType::COLOR, glm::vec3(1.0f)});
    m_Channels.push_back({"specular",  MaterialValueNodeEnum::SPECULAR, MaterialAttribType::COLOR, glm::vec3(.5f)});
    m_Channels.push_back({"shininess", MaterialValueNodeEnum::SHININESS, MaterialAttribType::FLOAT, 32.0f});
    m_Channels.push_back({"metalness", MaterialValueNodeEnum::METALNESS, MaterialAttribType::COLOR, glm::vec3(1.0f)});
    m_Channels.push_back({"roughness", MaterialValueNodeEnum::ROUGHNESS, MaterialAttribType::COLOR, glm::vec3(1.0f)});
    m_Channels.push_back({"normal",    MaterialValueNodeEnum::NORMAL, MaterialAttribType::COLOR, glm::vec3(1.0f)});
    m_Channels.push_back({"tile",      MaterialValueNodeEnum::TILE, MaterialAttribType::FLOAT, 0.0f});
}
DefaultGeometryMaterialTemplate::~DefaultGeometryMaterialTemplate() 
{
    m_Channels.clear();
}


MaterialTemplateType DefaultGeometryMaterialTemplate::GetType()
{
    return MaterialTemplateType::GEOMETRY;
}



ReflectiveMaterialTemplate::ReflectiveMaterialTemplate()
{
    m_Channels.push_back({"reflection", MaterialValueNodeEnum::REFLECTION, MaterialAttribType::FLOAT, 1.0f});
}

ReflectiveMaterialTemplate::~ReflectiveMaterialTemplate()
{
    m_Channels.clear();
}
MaterialTemplateType ReflectiveMaterialTemplate::GetType()
{
    return MaterialTemplateType::REFLECTIVE;
}

CustomMaterialTemplate::CustomMaterialTemplate()
{
    m_Channels.push_back({"shader", MaterialValueNodeEnum::CUSTOM, MaterialAttribType::STRING, ""});
}

CustomMaterialTemplate::~CustomMaterialTemplate()
{
     m_Channels.clear();
}
MaterialTemplateType CustomMaterialTemplate::GetType()
{
    return MaterialTemplateType::CUSTOM;
}


PointLightMaterialTemplate::PointLightMaterialTemplate()
{
    //m_Channels = {"ambient", "diffuse", "specular", "linear", "constant", "quadratic"};
    m_Channels.push_back({"ambient", MaterialValueNodeEnum::AMBIENT, MaterialAttribType::COLOR, glm::vec3(1.0f)});
    m_Channels.push_back({"diffuse", MaterialValueNodeEnum::DIFFUSE, MaterialAttribType::COLOR, glm::vec3(1.0f)});
    m_Channels.push_back({"specular", MaterialValueNodeEnum::SPECULAR, MaterialAttribType::COLOR, glm::vec3(.5f)});
    m_Channels.push_back({"linear", MaterialValueNodeEnum::LINEAR, MaterialAttribType::FLOAT, 0.09f});
    m_Channels.push_back({"constant", MaterialValueNodeEnum::CONSTANT, MaterialAttribType::FLOAT, 1.0f});
    m_Channels.push_back({"quadratic", MaterialValueNodeEnum::QUADRATIC, MaterialAttribType::FLOAT, 0.032f});
}
PointLightMaterialTemplate::~PointLightMaterialTemplate()
{
    m_Channels.clear();
}

MaterialTemplateType PointLightMaterialTemplate::GetType()
{
    return MaterialTemplateType::POINTLIGHT;
}

DirectionalLightMaterialTemplate::DirectionalLightMaterialTemplate()
{
    m_Channels.push_back({"ambient", MaterialValueNodeEnum::AMBIENT, MaterialAttribType::COLOR, glm::vec3(1.0f)});
    m_Channels.push_back({"diffuse", MaterialValueNodeEnum::DIFFUSE, MaterialAttribType::COLOR, glm::vec3(1.0f)});
    m_Channels.push_back({"specular", MaterialValueNodeEnum::SPECULAR, MaterialAttribType::COLOR, glm::vec3(.5f)});
}

DirectionalLightMaterialTemplate::~DirectionalLightMaterialTemplate()
{
    m_Channels.clear();
}

MaterialTemplateType DirectionalLightMaterialTemplate::GetType()
{
    return MaterialTemplateType::DIRECTIONALLIGHT;
}


std::vector<MaterialTemplateItem> MaterialTemplate::GetTemplate()
{
    return m_Channels;
}

void MaterialTemplate::AddVariationMask(std::byte mask)
{
    m_VariationMask |= mask;
}
bool MaterialTemplate::HasVariation(std::byte mask)
{
    return (short)(m_VariationMask & mask) > 0;
}

MaterialTemplate::~MaterialTemplate()
{
    m_Channels.clear();
}


WaterMaterialTemplate::WaterMaterialTemplate()
{
    m_Channels.push_back({"tint", MaterialValueNodeEnum::TINT, MaterialAttribType::COLOR, glm::vec3(0.0f, 0.2f, 0.3f)});
    m_Channels.push_back({"normal", MaterialValueNodeEnum::NORMAL, MaterialAttribType::COLOR, glm::vec3(.5f)});
    m_Channels.push_back({"transparency",MaterialValueNodeEnum::TRANSPARENCY, MaterialAttribType::FLOAT, 1.0f});
    m_Channels.push_back({"flow", MaterialValueNodeEnum::FLOW, MaterialAttribType::COLOR, glm::vec3(1.0f, 1.0f, 1.0f)});
    m_Channels.push_back({"waveStrength", MaterialValueNodeEnum::WAVE_STRENGTH, MaterialAttribType::FLOAT, 1.0f});
    m_Channels.push_back({"waveSpeed", MaterialValueNodeEnum::WAVE_SPEED, MaterialAttribType::COLOR, glm::vec3(1.0f, 1.0f, 1.0f)});
}


WaterMaterialTemplate::~WaterMaterialTemplate()
{
    m_Channels.clear();
}
MaterialTemplateType WaterMaterialTemplate::GetType()
{
    return MaterialTemplateType::WATER;
}

VolumeMaterialTemplate::VolumeMaterialTemplate()
{
    m_Channels.push_back({"tint", MaterialValueNodeEnum::TINT, MaterialAttribType::COLOR, glm::vec3(1.0, 1.0, 1.0)});
    m_Channels.push_back({"density", MaterialValueNodeEnum::DENSITY, MaterialAttribType::FLOAT, 1.0f});
}

VolumeMaterialTemplate::~VolumeMaterialTemplate()
{
    m_Channels.clear();
}

MaterialTemplateType VolumeMaterialTemplate::GetType()
{
    return MaterialTemplateType::VOLUME;
}


