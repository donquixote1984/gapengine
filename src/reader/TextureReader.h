#pragma once
#include <string>
#include <filesystem>
#include "../exceptions/ResourceNotFoundException.h"
#include "../textures/TextureType.h"
#include "../global/GlobalData.h"
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

class TextureReader
{
private:
    std::string m_TextureFolder;
    TexturePackage tp;
public:
    TextureReader(const std::string &textureFolder);
    TexturePackage GetTexturePackage();
};