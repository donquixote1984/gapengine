#include "Material.h"

Material::~Material(){}
Shader * Material::GetShader() const
{
    return m_IntrinsicShader;
}

void Material::AddShaderArgs(std::string key, std::string value)
{
    m_IntrinsicShader->AddArgs(key, value);
}

void Material::AddShaderArgs(std::map<std::string, std::string> map)
{
    m_IntrinsicShader->AddArgs(map);
}

void Material::AddShaderSnippet(ShaderSnippet ss)
{
    ss.ApplyToShader(m_IntrinsicShader);
}

void Material::AddShaderPartial(std::string key, std::string content)
{
    m_IntrinsicShader->AddPartials(key, content);
}
void Material::FlushToGlobalSettings()
{
    auto settings = settings::UISettings::GetSettings();
   //m_IntrinsicShader->setUniform1i("lightMode", settings->lightMode);
   // m_IntrinsicShader->setUniform1i("gamma", settings->gamma);
}


void Material::BindMaterialShaderAttrib(MaterialAttrib &attrib, unsigned int index) const
{
    m_IntrinsicShader->setUniform1i("material."+ std::string(attrib.uniform) + ".enable", (int)attrib.enable);
    if (attrib.type == MaterialAttribType::FLOAT)
    {
        m_IntrinsicShader->setUniform1f("material."+ std::string(attrib.uniform) + ".constant", attrib.constant);
    }
    if (attrib.type == MaterialAttribType::COLOR)
    {
        m_IntrinsicShader->setUniform3f("material."+ std::string(attrib.uniform) + ".color", attrib.color.x, attrib.color.y, attrib.color.z);

        Texture * texture = attrib.texture;
        if (texture != nullptr) 
        {
            texture->Bind(index);
            m_IntrinsicShader->setUniform1i("material."+std::string(attrib.uniform) + ".hasTexture", 1);
            m_IntrinsicShader->setUniform1i("material."+std::string(attrib.uniform) + ".tex", index);
        } else {
            m_IntrinsicShader->setUniform1i("material."+std::string(attrib.uniform) + ".hasTexture", 0); 
        }
        m_IntrinsicShader->setUniform1i("material." + std::string(attrib.uniform) + ".reverseY", attrib.reverseY);
    }
}

void Material::FlushToShader(ShaderUniformsCache& cache)
{
    FlushToShader();
    cache.FlushToShader(m_IntrinsicShader);
}