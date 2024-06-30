#pragma once
#include <string>
#include "../textures/CubeMapTexture.h"
#include "../shaders/Shader.h"
#include "../render/RenderContext.h"
#include "EnvBox.h"

class Skybox: public EnvBox
{
private:
    CubeMapTexture m_Texture;
public:
    ~Skybox();
    void ReadCubeMap(CubeMapTexturePath);
    CubeMapTexture GetTexture() const;
    void BindTexture(int slot = 0) override;
    void ActiveEnvironment() override;
};