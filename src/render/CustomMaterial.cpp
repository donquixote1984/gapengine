#include "CustomMaterial.h"

CustomMaterial::CustomMaterial(){}
CustomMaterial::CustomMaterial(const std::string shaderfile)
{
    m_IntrinsicShader = new Shader(shaderfile);
}
void CustomMaterial::FlushToShader() 
{
    for (auto &[k, v] : kv)
    {
        if (v.type == MaterialAttribType::COLOR) 
        {
            m_IntrinsicShader->setUniform3f(k, v.color.x, v.color.y, v.color.z);
        }

        if (v.type == MaterialAttribType::FLOAT)
        {
            m_IntrinsicShader->setUniform1f(k, v.constant);
        }
    }
}
void CustomMaterial::InitAttribs()
{}
CustomMaterial::~CustomMaterial()
{
    delete m_IntrinsicShader;
}