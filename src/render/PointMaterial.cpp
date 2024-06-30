#include "PointMaterial.h"


PointMaterial::PointMaterial()
{
    m_IntrinsicShader = GeometryShaderFactory::CreateGeometryShader(ShaderType::POINT_SHADER);
    InitAttribs();
}
void PointMaterial::InitAttribs()
{
    color = glm::vec3(0.0f, 1.0f, 0.0f);
    color.uniform = "color";

    size = 3.0f;
    size.uniform = "size";
}

void PointMaterial::FlushToShader() 
{
    m_IntrinsicShader->setUniform3f(color.uniform, color.color.x, color.color.y, color.color.z);
    m_IntrinsicShader->setUniform1f(size.uniform, size.constant);
}