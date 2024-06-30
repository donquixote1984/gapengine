#include "ReflectiveMaterial.h"


ReflectiveMaterial::ReflectiveMaterial()
{
    m_IntrinsicShader = GeometryShaderFactory::CreateGeometryShader(ShaderType::REFLECTIVE_SHADER);
    InitAttribs();
}

void ReflectiveMaterial::InitAttribs()
{
    reflection = 1.0f;
}

void ReflectiveMaterial::FlushToShader() 
{
    m_IntrinsicShader->Bind();
    m_IntrinsicShader->setUniform1f("reflection", reflection.constant);

    if (Global::scene -> GetEnvBox() != nullptr)
    {
        //unsigned int slot = Global::scene->GetEnvBox()->GetTexture().Bind();
        //m_IntrinsicShader->setUniform1i("skybox", slot);
        //m_IntrinsicShader->setUniform1i("hasSkybox", 1);
    } else {
        //m_IntrinsicShader->setUniform1i("hasSkybox", 0);
    }
}