#pragma once
#include <glm/glm.hpp>
#include <nlohmann/json.hpp>
#include <string>
#include "../util/JsonUtil.h"
#include "../global/ScreenProps.h"

using namespace nlohmann;

enum class VolumeShape
{
    SPHERE,
    BOX,
};
enum class VolumeType
{
    SDF,
    SIMPLE_CLOUD,
    CLOUD,
    TERRAIN,
};

struct VolumeMeta
{
    VolumeType type = VolumeType::SDF;
    std::string name = "";
    VolumeShape shape = VolumeShape::SPHERE;
    glm::vec3 size = glm::vec3(0);
    float canvasWidth = 0;
    float canvasHeight = 0;
    static VolumeMeta FromJson(const json &json)
    {
        VolumeMeta meta;
        if (json.contains("type"))
        {
            std::string type = json["type"];
            if (type == "cloud")
            {
                meta.type = VolumeType::CLOUD;
            }
            if (type == "simple_cloud")
            {
                meta.type = VolumeType::SIMPLE_CLOUD;
            }
            if (type == "sdf")
            {
                meta.type = VolumeType::SDF;
            }
            if (type == "terrain")
            {
                meta.type = VolumeType::TERRAIN;
            }
            if (type == "cloud")
            {
                meta.type = VolumeType::CLOUD;
            }
        }
        if (json.contains("shape"))
        {
            std::string shape = json["shape"];
            if (shape == "sphere")
            {
                meta.shape = VolumeShape::SPHERE;

                if (json.contains("radius"))
                {
                    float radius = json["radius"];
                    meta.size.x = radius;
                }
            }
            if (shape == "box")
            {
                meta.shape = VolumeShape::BOX;

                if (json.contains("size"))
                {
                    meta.size = util::GetPosition(json["size"]);
                }
            }
        }

        if (json.contains("width"))
        {
            meta.canvasWidth = json["width"];
        } else {
            meta.canvasWidth = Global::screen.width;
        }

        if (json.contains("height"))
        {
            meta.canvasHeight = json["height"];
        } else {
            meta.canvasHeight = Global::screen.height;
        }
        return meta;
    }
};