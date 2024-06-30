#include "CloudMaterial.h"
#include <GLFW/glfw3.h>
#include "../shaders/GeometryShader.h"

CloudMaterial::CloudMaterial()
{
    m_IntrinsicShader = GeometryShaderFactory::CreateGeometryShader(ShaderType::CLOUD);
    InitAttribs();
}
CloudMaterial::~CloudMaterial()
{
    delete m_IntrinsicShader;
    m_IntrinsicShader = nullptr;
}