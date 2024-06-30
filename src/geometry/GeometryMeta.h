#pragma once
#include <nlohmann/json.hpp>
#include <glm/glm.hpp>
#include "../util/JsonUtil.h"
#include "GeometryType.h"

using namespace nlohmann;

/**
 *  "type": "static", 
            "distribution": "circle",
            "count": 1000, 
            "scaleVariant": 0.2,
            "rotationVariant": {"x": 0, "y": 1, "z" : 0}
 * 
 * **/
struct GeometryMetaInstancingDirection
{
    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 forward;
};
struct GeometryMetaInstancing
{
    bool avail = false;
    unsigned int count = 0;
    unsigned int type = 0;
    unsigned int distribution = 0;
    float scaleVariant = 0;
    float radius = 0;
    int rows;
    int columns;
    float interval = 1.0f;

    GeometryMetaInstancingDirection direction;
    glm::vec3 offset = glm::vec3(0.0f);

    glm::vec3 rotationVariant = glm::vec3(0);

    static GeometryMetaInstancing FromJson(json instancingJson)
    {
        GeometryMetaInstancing gmi;
        if (instancingJson.contains("count"))
        {
            gmi.count = instancingJson["count"];
        }

        if (instancingJson.contains("rows"))
        {
            gmi.rows = instancingJson["rows"];
        }
        if (instancingJson.contains("offset"))
        {
            gmi.offset = util::GetPosition(instancingJson["offset"]);
        }

        if (instancingJson.contains("interval"))
        {
            gmi.interval = instancingJson["interval"];
        }
        if (instancingJson.contains("direction"))
        {
            json directionJson = instancingJson["direction"];
            if (directionJson.contains("up"))
            {
                gmi.direction.up = util::GetPosition(directionJson["up"]);
            }
            if (directionJson.contains("forward"))
            {
                gmi.direction.forward = util::GetPosition(directionJson["forward"]);
            }
        }
        if (instancingJson.contains("columns"))
        {
            gmi.columns = instancingJson["columns"];
        }
        if (instancingJson.contains("distribution"))
        {
            const std::string distribution = instancingJson["distribution"];
            gmi.distribution = (unsigned int)util::GetDistributionType(distribution);
        }
        if (instancingJson.contains("type"))
        {
            std::string type = instancingJson["type"];
            gmi.type = (unsigned int )util::GetInstancingType(type);
        }

        if (instancingJson.contains("scaleVariant"))
        {
            gmi.scaleVariant = instancingJson["scaleVariant"];
        }

        if (instancingJson.contains("rotationVariant"))
        {
            json pos = instancingJson["rotationVariant"];
            gmi.rotationVariant = util::GetPosition(pos);
        }

        if (instancingJson.contains("radius"))
        {
            float radius = instancingJson["radius"];
            gmi.radius = radius;
        }
        return gmi;
    }
};

struct GeometryMeta
{
    GeometryMetaInstancing instancing;
    GeometryType geoType = GeometryType::MESH;

    static GeometryMeta FromJson(json metajson)
    {
        GeometryMeta meta;
        if (metajson.contains("instancing"))
        {
            meta.instancing = GeometryMetaInstancing::FromJson(metajson["instancing"]);
            meta.instancing.avail = true;
        }

        if (metajson.contains("othermeta"))
        {
            //...
        }

        return meta;
    }
};