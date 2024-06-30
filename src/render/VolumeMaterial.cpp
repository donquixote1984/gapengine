
#include "VolumeMaterial.h"
#include "../shaders/GeometryShader.h"
#include "../global/ScreenProps.h"
#include <GLFW/glfw3.h>

VolumeMaterial::VolumeMaterial()
{
   m_IntrinsicShader = GeometryShaderFactory::CreateGeometryShader(ShaderType::VOLUME);
   InitAttribs();
}

VolumeMaterial::~VolumeMaterial()
{
    delete m_IntrinsicShader;
    m_IntrinsicShader = nullptr;
}
void VolumeMaterial::FlushToShader()
{
    m_IntrinsicShader->Bind();

    BindMaterialShaderAttrib(tint, 0);
    //BindMaterialShaderAttrib(density, 1);
    
    m_IntrinsicShader->setUniform1f("u_Time", static_cast<float>(glfwGetTime()));
}

void VolumeMaterial::InitAttribs()
{
    tint = glm::vec3(1.0f, 1.0f, 1.0f);
    tint.uniform = "tint";

}