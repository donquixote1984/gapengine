#pragma once
#include <vector>
#include <string>
#include "MaterialTemplateType.h"
#include "../util/JsonUtil.h"
#include "MaterialValueNodeEnum.h"
#include "../render/MaterialAttrib.h"
#include <glm/glm.hpp>
#include <any>
#include <memory>
#include <unordered_map>




struct MaterialTemplateItem
{
    std::string channelName;
    MaterialValueNodeEnum channelIndex;
    MaterialAttribType channelType;
    std::any defaultValue;
};

class MaterialTemplateVariation
{
public:
    const static std::byte PBR     = std::byte{0b00000010};
    const static std::byte BORDER  = std::byte{0b00000100};
    const static std::byte TERRAIN = std::byte{0b00001000};
    const static std::byte CLOUD   = std::byte{0b00010000};
    const static std::byte TERRAIN_SDF = std::byte{0b00100000};

    static std::byte FromString(std::string shading)
    {
        if (shading == "pbr")
        {
            return MaterialTemplateVariation::PBR;
        }
        if (shading == "border")
        {
            return MaterialTemplateVariation::BORDER;
        }
        if (shading == "terrain")
        {
            return MaterialTemplateVariation::TERRAIN;
        }

        if (shading == "simple_cloud")
        {
            return MaterialTemplateVariation::CLOUD;
        }
        if (shading == "cloud")
        {
            return MaterialTemplateVariation::CLOUD;
        }
        if (shading == "terrain_sdf")
        {
            return MaterialTemplateVariation::TERRAIN_SDF;
        }
        return std::byte{0b00000000};
    }

    static std::byte FromStrings(std::vector<std::string> shadings) 
    {
        std::byte ret = std::byte{0b00000000};
        for (std::string & s : shadings)
        {
            std::byte b = FromString(s);
            ret |= b;
        }
        return ret;
    }

};

class MaterialTemplate
{
protected:
    std::vector<MaterialTemplateItem> m_Channels;
    std::byte m_VariationMask = std::byte{0b00000000};
public:
    virtual ~MaterialTemplate();
    std::vector<MaterialTemplateItem> GetTemplate();
    virtual MaterialTemplateType GetType() = 0;
    void AddVariationMask(std::byte mask);
    bool HasVariation(std::byte);
};


class DefaultGeometryMaterialTemplate: public MaterialTemplate
{
public:
    DefaultGeometryMaterialTemplate();
    ~DefaultGeometryMaterialTemplate();
    MaterialTemplateType GetType() override;
};

class ReflectiveMaterialTemplate: public MaterialTemplate
{
public:
    ReflectiveMaterialTemplate();
    ~ReflectiveMaterialTemplate();
    MaterialTemplateType GetType() override;
};
class WaterMaterialTemplate: public MaterialTemplate
{
public:
    WaterMaterialTemplate();
    ~WaterMaterialTemplate();
    MaterialTemplateType GetType() override;
};

class VolumeMaterialTemplate: public MaterialTemplate
{
public:
    VolumeMaterialTemplate();
    ~VolumeMaterialTemplate();
    MaterialTemplateType GetType() override;
};

class PointLightMaterialTemplate: public MaterialTemplate
{
public:
    PointLightMaterialTemplate();
    ~PointLightMaterialTemplate();
    MaterialTemplateType GetType() override;
};

class CustomMaterialTemplate: public MaterialTemplate
{
public:
    CustomMaterialTemplate();
    ~CustomMaterialTemplate();
    MaterialTemplateType GetType() override;
};

class DirectionalLightMaterialTemplate: public MaterialTemplate
{
public:
    DirectionalLightMaterialTemplate();
    ~DirectionalLightMaterialTemplate();
    MaterialTemplateType GetType() override;
};




using namespace nlohmann;
class MaterialTemplateBuilder
{
public:
    static std::shared_ptr<MaterialTemplate> buildTemplate(json materialJson)
    {
        //  util::GetMaterialTemplateType(materialTemplateTypeStr);
        //util::GetColor();
        std::shared_ptr<MaterialTemplate> ret;
        std::string type = "";
        std::byte mask = std::byte{0b00000000};
        if (materialJson.contains("type"))
        {
            type = materialJson["type"];
        }
        if (materialJson.contains("shadings"))
        {
            if (materialJson["shadings"].is_array())
            {
                std::vector<std::string> shadings;
                for (nlohmann::json i : materialJson["shadings"])
                {
                    std::string shading = i;
                    shadings.push_back(i);
                }
                mask = MaterialTemplateVariation::FromStrings(shadings);
            }

            if (materialJson["shadings"].is_string())
            {
                std::string shading = materialJson["shadings"];
                mask = MaterialTemplateVariation::FromString(shading);
            }
        }

        MaterialTemplateType mtt = util::GetMaterialTemplateType(type);

        if (mtt == MaterialTemplateType::GEOMETRY)
        {
            ret = std::make_shared<DefaultGeometryMaterialTemplate>();
        }

        if (mtt == MaterialTemplateType::REFLECTIVE)
        {
            ret = std::make_shared<ReflectiveMaterialTemplate>();
        }

        if (mtt == MaterialTemplateType::DIRECTIONALLIGHT)
        {
            ret = std::make_shared<DirectionalLightMaterialTemplate>();
        }

        if (mtt == MaterialTemplateType::POINTLIGHT)
        {
            ret = std::make_shared<PointLightMaterialTemplate>();
        }

        if (mtt == MaterialTemplateType::WATER)
        {
            ret = std::make_shared<WaterMaterialTemplate>();
        }

        if (mtt == MaterialTemplateType::CUSTOM)
        {
            ret = std::make_shared<CustomMaterialTemplate>();
        }
        if (mtt == MaterialTemplateType::VOLUME)
        {
            ret = std::make_shared<VolumeMaterialTemplate>();
        }
        ret.get()->AddVariationMask(mask);
        return ret;
    }
};