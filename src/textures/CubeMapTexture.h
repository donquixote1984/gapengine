#pragma once
#include "glad/glad.h"
#include "stb/stb_image.h"
#include <string>
#include <iostream>

struct CubeMapTexturePath
{
    std::string top;
    std::string bottom;
    std::string left;
    std::string right;
    std::string front;
    std::string back;
};
class CubeMapTexture
{
private:
    unsigned int m_TextureId;
    void loadTexture(std::string path, unsigned int);
public:
    CubeMapTexture();
    void loadTextures(CubeMapTexturePath);
    unsigned int Bind();
};