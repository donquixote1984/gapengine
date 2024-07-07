#pragma once
#include <string>
#include <filesystem>
#include "../exceptions/ResourceNotFoundException.h"
#include "../textures/TextureType.h"
#include "../textures/Texture.h"
#include "../util/JsonUtil.h"

struct TexturePackage
{
    Texture * diffuse = nullptr;
    Texture * roughness = nullptr;
    Texture * metalness = nullptr;
    Texture * opacity = nullptr;
    Texture * normal = nullptr;
    Texture * specular = nullptr;
    Texture * ao = nullptr;
};
struct TexturePackagePath
{
    std::string diffuse = "";
    std::string roughness = "";
    std::string metalness = "";
    std::string opacity = "";
    std::string normal = "";
    std::string specular = "";
    std::string ao = "";
};

class TextureReader
{
private:
    std::string m_TextureFolder;
    TexturePackagePath tp;
public:
    TextureReader(const std::string &textureFolder);
    TexturePackagePath GetTexturePackage();
};