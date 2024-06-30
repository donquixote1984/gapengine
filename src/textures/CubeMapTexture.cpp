#include "CubeMapTexture.h"

CubeMapTexture::CubeMapTexture()
{}

void CubeMapTexture::loadTextures(CubeMapTexturePath paths)
{
    glGenTextures(1, &m_TextureId);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_TextureId);
    loadTexture(paths.right, 0);
    loadTexture(paths.left, 1);
    loadTexture(paths.top, 2);
    loadTexture(paths.bottom, 3);
    loadTexture(paths.front, 4);
    loadTexture(paths.back, 5);
}

void CubeMapTexture::loadTexture(std::string path, unsigned int index)
{
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(0);
    unsigned char *data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + index, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        stbi_image_free(data);
    }
    else
    {
        std::cout << "Cubemap texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }
    
}
unsigned int CubeMapTexture::Bind()
{
    int texture_units;
    glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &texture_units);
    glActiveTexture(GL_TEXTURE0 + texture_units-1);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_TextureId);
    return texture_units-1;
}