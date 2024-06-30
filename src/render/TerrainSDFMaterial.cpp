#include "TerrainSDFMaterial.h"
#include <GLFW/glfw3.h>
#include "../shaders/GeometryShader.h"
TerrainSDFMaterial::TerrainSDFMaterial()
{
    m_IntrinsicShader = GeometryShaderFactory::CreateGeometryShader(ShaderType::RAYMARCH_TERRAIN);
}
TerrainSDFMaterial::~TerrainSDFMaterial()
{
    delete m_IntrinsicShader;
}

void TerrainSDFMaterial::FlushToShader()
{
    m_IntrinsicShader->Bind();
    m_IntrinsicShader->setUniform1f("u_Time", static_cast<float>(glfwGetTime()));
}

void TerrainSDFMaterial::InitAttribs()
{
    
}