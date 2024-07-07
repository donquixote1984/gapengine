#include "TextureReader.h"
#include "../global/GlobalData.h"

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

               // xxx.color.jpg
        // xxx.roughness.png
        TextureType textureType = util::FromFilePath(textureRealPath);
        switch (textureType)
        {
            case TextureType::AO_TEXTURE:
                tp.ao        = textureRealPath;
                break;
            case TextureType::DIFFUSE_TEXTURE:
                tp.diffuse   = textureRealPath;
                break;
            case TextureType::METALNESS_TEXTURE:
                tp.metalness = textureRealPath;
                break;
            case TextureType::NORMAL_TEXTURE:
                tp.normal    = textureRealPath;
                break;
            case TextureType::ROUGHNESS_TEXTURE:
                tp.roughness = textureRealPath;
                break;
            case TextureType::SPECULAR_TEXTURE:
                tp.specular  = textureRealPath;
                break;
            case TextureType::OPACITY_TEXTURE:
                tp.opacity   = textureRealPath;
                break;
            default:
                break;
        }
    }
}

TexturePackagePath TextureReader::GetTexturePackage()
{
    return tp;
}