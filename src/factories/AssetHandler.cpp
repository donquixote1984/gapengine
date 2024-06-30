#include "AssetHandler.h"

AssetHandler::AssetHandler(const std::string &assetRelativepath, ObjPreProcessing opp):m_OPP(opp)
{
    if (!boost::algorithm::ends_with(assetRelativepath, constants::ASSET_EXT))
    {
        throw ResourceNotFoundException((assetRelativepath + " must ends with a .asset suffix").c_str());
    }

    
    m_AssetRelativePath = assetRelativepath;

    while(m_AssetRelativePath[0] == '/')
    {
        m_AssetRelativePath = m_AssetRelativePath.substr(1, m_AssetRelativePath.size() - 1);
    }

    m_AssetTempFullPath = util::GetAssetTempPath(m_AssetRelativePath);
    m_AssetName = util::GetFileName(m_AssetRelativePath);
    
    // real *.asset path
    m_AssetRealFullPath = constants::ASSET_INSTALL_PATH + assetRelativepath;

    if (!std::filesystem::exists(m_AssetRealFullPath))
    {
        throw ResourceNotFoundException((m_AssetTempFullPath + " does not exists.").c_str());
    }

    std::string modelsFolder   = m_AssetTempFullPath + "/" + constants::TEMP_MODEL_FODLER;
    std::string texturesFolder = m_AssetTempFullPath + "/" + constants::TEMP_TEX_FOLDER;

    if (std::filesystem::exists(modelsFolder) && std::filesystem::exists(texturesFolder))
    {
        //already extracted.
        //ResolveModel
    } else {
        // extract asset
        assetpacker::AssetFileExtractor afe(m_AssetTempFullPath);
        afe.Extract(m_AssetRealFullPath); // extract /temp/assets/models/grass/grass1/models/xxxx.model.fbx /temp/assets/models/grass/grass1/models/xxxx.color.jpg
        //ResolveModel();
    }
}

void AssetHandler::ResolveModel(Geometry *g)
{
    std::string modelsFolder =m_AssetTempFullPath+ "/" + constants::TEMP_MODEL_FODLER;
    VariantObjReader r(modelsFolder, m_OPP);
    r >> g;
}

void AssetHandler::operator >> (Geometry *g)
{
    ResolveModel(g);
}

void AssetHandler::operator >> (BasicMaterial * m)
{
    ResolveTex(m);
}

void AssetHandler::ResolveTex(BasicMaterial* m)
{
    std::string texFolder = m_AssetTempFullPath + "/" + constants::TEMP_TEX_FOLDER;
    TextureReader tr(texFolder);
    TexturePackage tp = tr.GetTexturePackage();
    if (tp.ao != nullptr) 
    {
        m->ambient.texture = tp.ao;
    }
    
    if (tp.diffuse != nullptr)
    {
        m->diffuse.texture = tp.diffuse;
    }

    if (tp.metalness != nullptr)
    {
        m->metalness.texture = tp.metalness;
    }

    if (tp.normal != nullptr)
    {
        m->normal.texture = tp.normal;
        m->SetTangent(true);
    }
    if (tp.roughness != nullptr)
    {
        m->roughness.texture = tp.roughness;
    }

    if (tp.specular != nullptr)
    {
        m->specular.texture = tp.specular;
    }
}

bool AssetHandler::ValidateModel()
{
    //TODO
    return true;
}

bool AssetHandler::ValidateTex()
{
    //TODO
    return true;
}