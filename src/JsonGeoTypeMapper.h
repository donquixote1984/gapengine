#pragma once
#include <tuple>
#include "geometry/Geometry.h"
#include "geometry/GeometryType.h"
#include "lights/Light.h"
#include "primitives/PrimitiveType.h"
#include "exceptions/InvalidTypeException.h"
#include "fmt/format.h"

class JsonGeoTypeMapper
{
public: 
    
    static std::tuple<GeometryType, unsigned int> GetType(std::string type, std::string subtype)
    {
        if (type == "mesh" || type == "instancing")
        {
            return std::make_tuple(GeometryType::MESH, 0u);
        }
        if (type == "terrain")
        {
            return std::make_tuple(GeometryType::TERRAIN, 0u);
        }
        if (type == "pointcloud")
        {
            return std::make_tuple(GeometryType::PC, 0u);
        }
        if (type == "container")
        {
            return std::make_tuple(GeometryType::CONTAINER, 0u);
        }

        if (type == "water")
        {
            return std::make_tuple(GeometryType::WATER, 0u);
        }

        if (type == "volume")
        {
            return std::make_tuple(GeometryType::VOLUME, 0u);
        }

        if (type == "primitive")
        {
            PrimitiveType t;
            if (subtype == "cube")
            {
                t = PrimitiveType::CUBE;
            }

            if (subtype == "plane")
            {
                t = PrimitiveType::PLANE;
            }

            if (subtype== "sphere")
            {
                t = PrimitiveType::SPHERE;
            }
            return std::make_tuple(GeometryType::PRIMITIVE, static_cast<unsigned int>(t));
        }
        std::string exp = fmt::format("Unsupported Geo Type: {}", type);
        throw InvalidTypeException(exp.c_str());
    }
    static std::tuple<LightType, unsigned int> GetLightType(std::string lightType)
    {
        std::string t;
        if (lightType == "point")
        {
            return std::make_tuple(LightType::POINT, 0);
        }
        if (lightType == "directional")
        {
            return std::make_tuple(LightType::DIRECTIONAL, 0);
        }

        return std::make_tuple(LightType::LIGHT_RAW, 0);
        
    }
};