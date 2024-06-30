#pragma once
#include <string>
#include <filesystem>
#include "../Config.h"
#include "../geometry/Geometry.h"
#include "../geometry/GeometryData.h"
#include "../global/GlobalData.h"
#include "../exceptions/ResourceNotFoundException.h"
#include "../util/Util.h"
#include "ObjReader.h"
#include "ObjPreProcessing.h"
class VariantObjReader
{
private:
    std::string m_VariantFolder;
    int m_VariantSize = 0;
    ObjPreProcessing m_OPP;
public:
    VariantObjReader(const std::string &variantFolder, ObjPreProcessing opp);
    void operator>>(Geometry *g);
};