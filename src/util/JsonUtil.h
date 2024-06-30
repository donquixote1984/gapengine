#pragma once

#include <glm/glm.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include <nlohmann/json.hpp>
#include <initializer_list>
#include <string>
//#include "../node/MaterialValueNode.h"
#include "../geometry/DisplayMode.h"
#include "../node/MaterialTemplateType.h"
#include "../geometry/postprocess/DataPostProcessing.h"
#include "../textures/TextureType.h"
#include "../geometry/generator/PointGeneratorDistributionType.h"
#include "../geometry/GeometryInstancingType.h"
#include "../Config.h"
#include "../reader/ObjPreProcessing.h"

namespace util {
    glm::vec3 GetPosition( nlohmann::json pos);

    glm::vec3 GetColor(nlohmann::json color);

    DisplayMode GetDisplayMode(nlohmann::json mode);
    void test();
    MaterialTemplateType GetMaterialTemplateType(std::string materialType);
    const std::string GetAssetTempPath(const std::string &assetpath);
    const std::string GetFileName(const std::string &assetpath);
    const PostProcessingType GetPPType(const std::string & ppstr);
    TextureType FromFilePath(const std::string &filepath);
    TextureType GetTextureType(const std::string &type);
    GeometryInstancingType GetInstancingType(const std::string &type);
    PointGeneratorDistributionType GetDistributionType(const std::string &type);

    ObjPreProcessing GetObjPreProcessing(nlohmann::json preprocessing);
};