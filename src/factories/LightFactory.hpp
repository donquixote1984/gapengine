#pragma once
#include "../lights/Light.h"
#include <nlohmann/json.hpp>
#include "../lights/PointLight.h"
#include "../lights/DirectionalLight.h"
#include "../JsonGeoTypeMapper.h"
#include "../util/JsonUtil.h"
#include "../node/PointLightMaterialValueNode.h"
#include "../node/MaterialTemplate.h"
#include "../node/MaterialNodeReader.hpp"
#include "../render/LightMaterial.h"

#include <memory>

class LightFactory
{

public:
    virtual ~LightFactory() {};
    virtual Light * CreateLight(nlohmann::json lightjson) = 0;
};

class PointLightFactory: public LightFactory
{
public:
    Light * CreateLight(nlohmann::json lightjson)
    {
        PointLight* l = new PointLight();
        //glm::vec3 color = util::GetColor(lightjson["color"]);
        glm::vec3 point = util::GetPosition(lightjson["position"]);
        l->SetPosition(point);
        nlohmann::json materialJson = lightjson["material"];

        MaterialTemplate *pmt = new PointLightMaterialTemplate();

        std::shared_ptr<MaterialValueNode> mvn = MaterialNodeReader::ReadMaterial(materialJson, pmt);
        LightMaterial* lm = static_cast<LightMaterial*> ( mvn.get()->GetFirstOutputValue().GetPtr());
        l->SetMaterial(lm);
        
        delete pmt;
        return l;
    }
};

class DirectionalLightFactory: public LightFactory
{
public:
    Light * CreateLight(nlohmann::json lightjson)
    {
        Light * l = new DirectionalLight();
        glm::vec3 dir = util::GetPosition(lightjson["direction"]);
        l->SetDirection(dir);

        nlohmann::json materialJson = lightjson["material"];
        MaterialTemplate *pmt = new DirectionalLightMaterialTemplate();
        std::shared_ptr<MaterialValueNode> mvn = MaterialNodeReader::ReadMaterial(materialJson, pmt);
        LightMaterial* lm = static_cast<LightMaterial*> ( mvn.get()->GetFirstOutputValue().GetPtr());
        l->SetMaterial(lm); 
        delete pmt;
        return l;
    }
};
