#include "JsonUtil.h"
glm::vec3 util::GetPosition( nlohmann::json pos)
{
    float x = pos.contains("x") ? (float)pos["x"] : 0;
    float y = pos.contains("y") ? (float)pos["y"] : 0;
    float z = pos.contains("z") ? (float)pos["z"] : 0;
    return {x, y, z};
}

glm::vec3 util::GetColor(nlohmann::json color)
{
    float r = color["r"];
    float g = color["g"];
    float b = color["b"];
    return glm::vec3(r,g,b);
}

DisplayMode util::GetDisplayMode(nlohmann::json mode)
{
    std::string modeStr = mode;
    if (modeStr == "wireframe")
    {
        return DisplayMode::WIREFRAME;
    }

    if(modeStr == "points")
    {
        return DisplayMode::POINTS;
    }

    return DisplayMode::MESH;
}
MaterialTemplateType util::GetMaterialTemplateType(std::string materialType)
{
    if (materialType.empty())
    {
        return MaterialTemplateType::GEOMETRY;
    }

    if (materialType == "reflect")
    {
        return MaterialTemplateType::REFLECTIVE;
    }

    if (materialType == "pointlight")
    {
        return MaterialTemplateType::POINTLIGHT;
    }

    if (materialType == "directionallight") 
    {
        return MaterialTemplateType::DIRECTIONALLIGHT;
    }

    if (materialType == "water")
    {
        return MaterialTemplateType::WATER;
    }

    if (materialType == "custom")
    {
        return MaterialTemplateType::CUSTOM;
    }

    if (materialType == "volume")
    {
        return MaterialTemplateType::VOLUME;
    }

    return MaterialTemplateType::UNSUPPORTED;
}


const std::string util::GetAssetTempPath(const std::string &assetpath)
{
    std::string assetTempPath;
    if (boost::algorithm::ends_with(assetpath, constants::ASSET_EXT))
    {
        assetTempPath = assetpath.substr(0, assetpath.size() - 6);
    }

    while(assetTempPath[0] == '/')
    {
        assetTempPath = assetTempPath.substr(1, assetTempPath.size()-1);
    }
    
    std::string fullpath = constants::TEMP_ASSET_FOLDER_NAME + assetTempPath;
    return fullpath;
}
const std::string util::GetFileName(const std::string &assetpath)
{
    std::string base_filename = assetpath.substr(assetpath.find_last_of("/\\") + 1);
    if (boost::algorithm::ends_with(base_filename, constants::ASSET_EXT))
    {
        base_filename = base_filename.substr(0, base_filename.size() - 6);
    }
    return base_filename;
}

const PostProcessingType util::GetPPType(const std::string & ppstr)
{
    if (ppstr == "scatter")
    {
        return PostProcessingType::SCATTER;
    }

    if (ppstr == "noise")
    {
        return PostProcessingType::NOISE;
    }

    if (ppstr == "replica")
    {
        return PostProcessingType::REPLICA;
    }

    if (ppstr == "pointonly")
    {
        return PostProcessingType::POINTONLY;
    }

    return PostProcessingType::NONE;

}


void util::test()
{}

TextureType util::FromFilePath(const std::string &filepath)
{
    // extract last '/'
    std::string filename = util::GetFileName(filepath);
    
    boost::algorithm::to_lower(filename);
    if (filename.find("normal") != std::string::npos)
    {
        return TextureType::NORMAL_TEXTURE;
    }

    if (filename.find(".color") != std::string::npos)
    {
        return TextureType::DIFFUSE_TEXTURE;
    }

    if (filename.find(".ao.") != std::string::npos)
    {
        return TextureType::AO_TEXTURE;
    }


    if (filename.find(".specular") != std::string::npos)
    {
        return TextureType::SPECULAR_TEXTURE;
    }

    if (filename.find(".roughness") != std::string::npos)
    {
        return TextureType::ROUGHNESS_TEXTURE;
    }

    if (filename.find(".metalness") != std::string::npos)
    {
        return TextureType::METALNESS_TEXTURE;
    }

    if (filename.find(".opacity") != std::string::npos)
    {
        return TextureType::OPACITY_TEXTURE;
    }

    return TextureType::NO_TEXTURE;
}

TextureType util::GetTextureType(const std::string &type)
{
    if (type == "diffuse")
        return TextureType::DIFFUSE_TEXTURE;
    if (type == "roughness")
        return TextureType::ROUGHNESS_TEXTURE;
    if (type == "normal")
        return TextureType::NORMAL_TEXTURE;
    if (type == "ao")
        return TextureType::AO_TEXTURE;
    if (type == "metalness")
        return TextureType::METALNESS_TEXTURE;
    if (type == "specular")
        return TextureType::SPECULAR_TEXTURE;
    if (type == "opacity")
        return TextureType::OPACITY_TEXTURE;
    return TextureType::NO_TEXTURE;
}

GeometryInstancingType util::GetInstancingType(const std::string &type)
{
    if (type == "static")
    {
        return GeometryInstancingType::STATIC;
    }

    if (type == "inherit")
    {
        return GeometryInstancingType::INHERIT;
    }
    return GeometryInstancingType::STATIC;
}

PointGeneratorDistributionType util::GetDistributionType(const std::string &type)
{
    if (type == "circle")
    {
        return PointGeneratorDistributionType::CIRCLE;
    }

    if (type == "square")
    {
        return PointGeneratorDistributionType::SQUARE;
    }
    if (type == "grid")
    {
        return PointGeneratorDistributionType::GRID;
    }

    return PointGeneratorDistributionType::CIRCLE;
}


ObjPreProcessing util::GetObjPreProcessing(nlohmann::json preprocessing)
{
    ObjPreProcessing opp;
    if (preprocessing.contains("unitscale"))
    {
        float unitScale = preprocessing["unitscale"];
        opp.unitScale = unitScale;
    }
    if (preprocessing.contains("withAnimation"))
    {
        bool withAnimation = preprocessing["withAnimation"];
        opp.withAnimation = withAnimation;
    }
    return opp;
}