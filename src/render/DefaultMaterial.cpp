#include "DefaultMaterial.h"
#include "../shaders/GeometryShader.h"
DefaultMaterial::DefaultMaterial(): BasicMaterial()
{
    m_IntrinsicShader = GeometryShaderFactory::CreateGeometryShader(ShaderType::DEFAULT_SHADER);
    InitDefaultSnippets();
};