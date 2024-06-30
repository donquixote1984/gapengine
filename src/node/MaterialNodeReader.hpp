#pragma once

#include "MaterialValueNode.h"
#include "GeometryMaterialValueNode.h"
#include "PointLightMaterialValueNode.h"
#include "DirectionalLightMaterialValueNode.h"
#include "CustomMaterialValueNode.h"
#include "ReflectiveMaterialValueNode.h"
#include "WaterMaterialValueNode.h"
#include "VolumeMaterialValueNode.h"
#include <nlohmann/json.hpp>
#include "../util/JsonUtil.h"
#include "MaterialTemplate.h"
#include <memory>

using namespace nlohmann;

class MaterialNodeReader
{
private:
    static std::shared_ptr<MaterialValueNode> GetMaterialValueNode(MaterialTemplate *mt)
    {
        if (mt->GetType() == MaterialTemplateType::GEOMETRY)
        {
            return std::make_shared<GeometryMaterialValueNode>(mt);
        }
        if (mt->GetType() == MaterialTemplateType::REFLECTIVE)
        {
            return std::make_shared<ReflectiveMaterialValueNode>(mt);
        }
        if (mt->GetType() == MaterialTemplateType::POINTLIGHT)
        {
            return std::make_shared<PointLightMaterialValueNode>(mt);
        }

        if (mt->GetType() == MaterialTemplateType::DIRECTIONALLIGHT)
        {
            return std::make_shared<DirectionalLightMaterialValueNode>(mt);
        }

        if (mt->GetType() == MaterialTemplateType::WATER)
        {
            return std::make_shared<WaterMaterialValueNode>(mt);
        }

        if (mt->GetType() == MaterialTemplateType::VOLUME)
        {
            return std::make_shared<VolumeMaterialValueNode>(mt);
        }

        if (mt->GetType() == MaterialTemplateType::CUSTOM)
        {
            return std::make_shared<CustomMaterialValueNode>(mt);
        }

        return nullptr;
    }
public:
    static std::shared_ptr<MaterialValueNode> ReadMaterial(json materialJson, MaterialTemplate * mt)
    {
        std::vector<MaterialTemplateItem> channels = mt->GetTemplate();

        std::shared_ptr<MaterialValueNode> mvn = GetMaterialValueNode(mt);

        for (int i = 0; i < channels.size(); i++)
        {
            std::string channelName = channels[i].channelName;
            json channelJson = materialJson[channelName];
            MaterialNodeReader::ChannelResolver(channelJson, (unsigned int)channels[i].channelIndex, mvn.get());
        }
        return mvn;
    }

    static void ChannelResolver(json channelJson, unsigned int channelKey, MaterialValueNode *mvn)
    {
        json baseColorJson = NULL; 
        json textureJson = NULL; 
        json constantJson = NULL;
        std::string shader;

        if (channelJson.type() == json::value_t::string)
        {
            std::string channel = channelJson;
            StringValueNode *svn = new StringValueNode(channel);
            mvn->UpdateInputValue(channelKey, svn);
            return;
        }

        if (channelJson.contains("basecolor"))
        {
            baseColorJson = channelJson["basecolor"];
        }

        if (channelJson.contains("texture"))
        {
            textureJson = channelJson["texture"];
        }

        if (channelJson.contains("constant"))
        {
            constantJson = channelJson["constant"];
        }

        if (constantJson != NULL) 
        {
            float constant = constantJson;
            FloatValueNode * fvn = new FloatValueNode(constant);
            mvn->UpdateInputValue(channelKey, fvn);
            return;
        }

        // create new color node by UI
        ColorValueNode *cvn = new ColorValueNode();
        
        if (baseColorJson != NULL)
        {
            glm::vec3 baseColor = util::GetColor(baseColorJson);
            cvn->AttachColor(baseColor.r, baseColor.g, baseColor.b);
        }

        if (textureJson != NULL)
        {
            std::string filePath = textureJson;
            cvn->AttachTexture(filePath);
        }
        mvn->UpdateInputValue(channelKey, cvn);
    }
};