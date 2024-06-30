#include "DirectionalLightMaterial.h"
#include "../shaders/GeometryShader.h"

DirectionalLightMaterial::DirectionalLightMaterial()
{
    GeometryShaderFactory::CreateGeometryShader(ShaderType::DIRECTIONAL_LIGHT_SHADER);
}
void DirectionalLightMaterial::InitAttribs()
{}