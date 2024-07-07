#include "VariantObjReader.h"

VariantObjReader::VariantObjReader(const std::string &variantFolder, ObjPreProcessing opp):m_OPP(opp)
{
    m_VariantFolder = variantFolder;
}

void VariantObjReader::operator >> (Geometry* g)
{
    if(!std::filesystem::exists(m_VariantFolder))
    {
        throw ResourceNotFoundException(m_VariantFolder.c_str());
    }
    
    int filecount = util::FileCount(m_VariantFolder);
    if (filecount == 0)
    {
        return;
    }
    if(Global::geometryDataMap.find(m_VariantFolder) != Global::geometryDataMap.end())
    {

        GeometryData *variableGeometryData = Global::geometryDataMap[m_VariantFolder];
        uint8_t size = 0;
        for (uint8_t i = 0; i < constants::MAX_GEO_VARIANT; i++)
        {
            if (!variableGeometryData[i].IsEmpty())
            {
                size += 1;
            }
        }
        g->InitGeometryVariant(variableGeometryData, size);
        return;
    }

    GeometryData *variableGeometryData = GeometryDataFactory::CreateEmptyGeometryVariantData(constants::MAX_GEO_VARIANT);//new GeometryData[constants::MAX_GEO_VARIANT];

    //std::fill(std::begin(variableGeometryData), std::begin(variableGeometryData) + constants::MAX_GEO_VARIANT, );

    for(const auto & entry: std::filesystem::directory_iterator(m_VariantFolder))
    {
        std::string modelVariant = entry.path().string();
        ObjReader r(modelVariant, m_OPP);
        if (r.GetMeshes().size() > 0)
        {
            variableGeometryData[m_VariantSize].FeedData(r.GetMeshes()); 
            variableGeometryData[m_VariantSize].SetUseAssetMaterial(m_OPP.useAssetMaterial);
            m_VariantSize += 1;
        }
    }
    g->InitGeometryVariant(variableGeometryData, m_VariantSize);
}
