#pragma once
#include <tuple>
#include "GeometryFactory.hpp"
#include <nlohmann/json.hpp>
#include "../JsonGeoTypeMapper.h"
#include "../geometry/GeometryType.h"

class GeometryFactoryBuilder
{
public:
    static GeometryFactory * GetFactory(nlohmann::json &geojson)
    {
        std::string typeStr;
        std::string subtypeStr;

        if (geojson.contains("type"))
        {
            typeStr = geojson["type"];
        }

        if (geojson.contains("subtype"))
        {
            subtypeStr = geojson["subtype"];
        }

        auto [geoType, subType] = JsonGeoTypeMapper::GetType(typeStr, subtypeStr);


        if (geoType == GeometryType::MESH)
        {
            return new MeshGeometryFactory();
        }
        if (geoType == GeometryType::PRIMITIVE)
        {
            //return new PlainGeometry();
            return new PrimitiveGeometryFactory();
        }
        if (geoType == GeometryType::CONTAINER)
        {
            return new ContainerGeometryFactory();
        }
        if (geoType == GeometryType::TERRAIN)
        {
            return new TerrainGeometryFactory();
        }
        if (geoType == GeometryType::WATER)
        {
            return new WaterGeometryFactory();
        }
        if (geoType == GeometryType::VOLUME)
        {
            return new VolumeGeometryFactory();
        }

        std::string exp = static_cast<std::string>(magic_enum::enum_name(geoType));
        throw InvalidTypeException(exp.c_str());
    }
};
