#include "WaterMaterial.h"
#include "../shaders/GeometryShader.h"
#include "../textures/Texture.h"
#include <GLFW/glfw3.h>
WaterMaterial::WaterMaterial()
{
   m_IntrinsicShader = GeometryShaderFactory::CreateGeometryShader(ShaderType::WATER);
   InitAttribs();
};
WaterMaterial::~WaterMaterial()
{
    delete m_IntrinsicShader;
    m_IntrinsicShader = nullptr;
}

void WaterMaterial::FlushToShader() 
{
    m_IntrinsicShader->Bind();

    BindMaterialShaderAttrib(tint, 0);
    BindMaterialShaderAttrib(transparency, 1);
    BindMaterialShaderAttrib(flow, 2);
    BindMaterialShaderAttrib(waveStrength, 3);
    BindMaterialShaderAttrib(waveSpeed, 4);
    BindMaterialShaderAttrib(normal, 5);

    m_IntrinsicShader->setUniform1f("time", static_cast<float>(glfwGetTime()));
}

void WaterMaterial::InitAttribs()
{
    tint = glm::vec3(0.0f, 0.2f, 0.3f);
    tint.uniform = "tint";

    transparency = 1.0f;
    transparency.uniform = "transparency";

    flow = glm::vec3(1.0f, 1.0f, 1.0f);
    flow.uniform = "flow";
    
    waveStrength = 1.0f;
    waveStrength.uniform = "waveStrength";
    //InitDefaultSnippets();
    waveSpeed = glm::vec3(1.0f, 1.0f, 1.0f);
    waveSpeed.uniform = "waveSpeed";

    normal = glm::vec3(1.0f, 1.0f, 1.0f);
    normal.uniform = "normal";
}