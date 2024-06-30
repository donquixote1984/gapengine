#include "PointLightMaterial.h"
#include "../shaders/GeometryShader.h"
PointLightMaterial::PointLightMaterial()
{
    GeometryShaderFactory::CreateGeometryShader(ShaderType::POINT_LIGHT_SHADER);
}
void PointLightMaterial::InitAttribs()
{}