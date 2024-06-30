#include "glad/glad.h"
#include "Texture.h"


Texture::Texture():  m_RendererID(0), m_Width(0), m_Height(0), m_BPP(0), hasTexture(false)
{}

Texture::Texture(const std::string & path, bool useMipmap, bool flipped) : m_RendererID(0), m_Width(0), m_Height(0), m_BPP(0)
{
    BindTexture(path, useMipmap, flipped);
}
Texture::Texture(MemoryTextureData data, bool useMipmap, bool flipped) : m_RendererID(0), m_Width(0), m_Height(0), m_BPP(0)
{
    BindTexture(data, useMipmap, flipped);
}

Texture::~Texture()
{
    glDeleteTextures(1, &m_RendererID);
}

void Texture::BindTexture(const std::string &path, bool useMipmap, bool flipped)
{
    int width, height, bpp, nchannels;
    unsigned char * buffer = stbi_load(path.c_str(), &width, &height, &bpp, 4);
    MemoryTextureData data = {buffer, width, height, 4};
    BindTexture(data, useMipmap, flipped);
}
void Texture::BindTexture(MemoryTextureData data, bool useMipmap, bool flipped)
{
    hasTexture = true;
    m_Width = data.width;
    m_Height = data.height;

    glGenTextures(1, &m_RendererID);
    glBindTexture(GL_TEXTURE_2D, m_RendererID);

    if (flipped)
    {
        stbi_set_flip_vertically_on_load(1);
    }

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_REPEAT);

    if (useMipmap) 
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    } else {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    }

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data.data);

    if (useMipmap)
    {
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    glBindTexture(GL_TEXTURE_2D, 0);

    if (data.data)
    {
        stbi_image_free(data.data);
    }
    stbi_set_flip_vertically_on_load(0);
}

void Texture::Bind(unsigned int slot) const
{
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, m_RendererID);
}

void Texture::UnBind() const
{

}
unsigned int Texture::getTextureId() const {
    return m_RendererID;
}