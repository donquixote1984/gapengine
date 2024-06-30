#include "TerrainMaterial.h"
#include "../shaders/GeometryShader.h"
TerrainMaterial::TerrainMaterial(): BasicMaterial()
{
    m_IntrinsicShader = GeometryShaderFactory::CreateGeometryShader(ShaderType::TERRAIN_SHADER);
    InitDefaultSnippets();
};
TerrainMaterial::~TerrainMaterial()
{
    delete m_IntrinsicShader;
    m_IntrinsicShader = nullptr;
}