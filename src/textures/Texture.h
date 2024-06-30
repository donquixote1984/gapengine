#pragma once
#include "TextureType.h"
#include <string>
#include "MemoryTextureData.h"
#include "stb/stb_image.h"



class Texture
{
private:
    unsigned int m_RendererID;
    int m_Width, m_Height, m_BPP;
    bool hasTexture = false;
    std::string m_ShaderSlot;

public:
    Texture();
    Texture(const std::string & path, bool useMipmap = true, bool flipped = false);
    Texture(MemoryTextureData data, bool useMipmap = true, bool flipped = false);
    ~Texture();
    void BindTexture(const std::string &path, bool useMipmap, bool flipped);
    void BindTexture(MemoryTextureData data, bool useMipmap, bool flipped);
    void Bind(unsigned int slot = 0) const;
    void UnBind() const;
    inline int GetWidth() const { return m_Width;}
    inline int GetHeight() const {return m_Height;}
    unsigned int getTextureId() const;
};