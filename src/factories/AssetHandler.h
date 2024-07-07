#pragma once

#include <boost/algorithm/string/predicate.hpp>
#include <filesystem>
#include <string>
#include "../assetpacker/AssetFileExtractor.h"
#include "../reader/VariantObjReader.h"
#include "../geometry/Geometry.h"
#include "../render/BasicMaterial.h"
#include "../exceptions/ResourceNotFoundException.h"
#include "../util/JsonUtil.h"
#include "../Config.h"
#include "../reader/TextureReader.h"
#include "../reader/ObjPreProcessing.h"

class AssetHandler
{
private:
    std::string m_AssetRelativePath;
    std::string m_AssetName;
    std::string m_AssetTempFullPath;
    std::string m_AssetRealFullPath;
    ObjPreProcessing m_OPP;
    void ResolveModel(Geometry *g);
    bool ValidateModel();
    bool ValidateTex();
public:
    AssetHandler(const std::string &assetpath, ObjPreProcessing opp);
    void operator >> (Geometry *g);
};