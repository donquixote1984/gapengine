#include "PBRMaterial.h"
#include "../shaders/GeometryShader.h"
PBRMaterial::PBRMaterial(): BasicMaterial()
{
    m_IntrinsicShader = GeometryShaderFactory::CreateGeometryShader(ShaderType::PBR_SHADER);
    InitDefaultSnippets();
};