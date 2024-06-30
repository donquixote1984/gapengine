#pragma once
#include <nlohmann/json.hpp>
#include "LightFactory.hpp"
#include "../exceptions/InvalidTypeException.h"
#include "fmt/format.h"

LightType GetLightType(std::string type)
{
    if (type == "point")
    {
        return LightType::POINT;
    }

    if (type == "direction")
    {
        return LightType::DIRECTIONAL;
    }
    std::string exp = fmt::format("Invalid Light Type : {}", type);
    throw  InvalidTypeException(exp.c_str());
}

class LightFactoryBuilder
{
public:
    static LightFactory * GetFactory(nlohmann::json geojson)
    {
        std::string typeStr = geojson["type"];
        LightType type = GetLightType(typeStr);
        if (type == LightType::POINT)
        {
            return new PointLightFactory();
        }

        if (type == LightType::DIRECTIONAL)
        {
            return new DirectionalLightFactory();
        }
        return nullptr;
    }
};