#include "SimpleCloudMaterial.h"
#include <GLFW/glfw3.h>
#include "../shaders/GeometryShader.h"
SimpleCloudMaterial::SimpleCloudMaterial()
{
    m_IntrinsicShader = GeometryShaderFactory::CreateGeometryShader(ShaderType::SIMPLE_CLOUD);
    InitAttribs();
}
SimpleCloudMaterial::~SimpleCloudMaterial()
{
    delete m_IntrinsicShader;
    m_IntrinsicShader = nullptr;
}

void SimpleCloudMaterial::FlushToShader()
{
    m_IntrinsicShader->Bind();

    BindMaterialShaderAttrib(tint, 0);
    BindMaterialShaderAttrib(density, 1);
    
    m_IntrinsicShader->setUniform1f("u_Time", static_cast<float>(glfwGetTime()));
}

void SimpleCloudMaterial::InitAttribs()
{
    tint = glm::vec3(1.0f, 1.0f, 1.0f);
    tint.uniform = "tint";

    density = 1.0f;
    density.uniform = "density";
}