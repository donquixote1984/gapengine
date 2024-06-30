#include "TextureReader.h"

TextureReader::TextureReader(const std::string &textureFolder)
{

    if (!std::filesystem::exists(textureFolder))
    {
        throw ResourceNotFoundException(textureFolder.c_str());
    }
    m_TextureFolder = textureFolder;
    for(auto const & entry: std::filesystem::directory_iterator(textureFolder))
    {
        std::string textureRealPath = entry.path().string();
        Texture *t;
        if (Global::texturesMap.find(textureRealPath) != Global::texturesMap.end())
        {
            t = Global::texturesMap[textureRealPath];
        } else {
            t = new Texture(textureRealPath);
            Global::texturesMap[textureRealPath] = t;
        }
        // xxx.color.jpg
        // xxx.roughness.png
        TextureType textureType = util::FromFilePath(textureRealPath);
        switch (textureType)
        {
            case TextureType::AO_TEXTURE:
                tp.ao        = t;
                break;
            case TextureType::DIFFUSE_TEXTURE:
                tp.diffuse   = t;
                break;
            case TextureType::METALNESS_TEXTURE:
                tp.metalness = t;
                break;
            case TextureType::NORMAL_TEXTURE:
                tp.normal    = t;
                break;
            case TextureType::ROUGHNESS_TEXTURE:
                tp.roughness = t;
                break;
            case TextureType::SPECULAR_TEXTURE:
                tp.specular  = t;
                break;
            case TextureType::OPACITY_TEXTURE:
                tp.opacity   = t;
                break;
            default:
                break;
        }
    }
}

TexturePackage TextureReader::GetTexturePackage()
{
    return tp;
}