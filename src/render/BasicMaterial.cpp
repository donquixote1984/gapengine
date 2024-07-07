#include "BasicMaterial.h"
#include "../shaders/GeometryShader.h"
#include "../textures/Texture.h"
BasicMaterial::BasicMaterial()
{
   InitAttribs();
};
BasicMaterial::~BasicMaterial()
{}

void BasicMaterial::FlushToShader() 
{
    m_IntrinsicShader->Bind();

    normal.enable = settings::UISettings::GetSettings()->normal == 1;

    BindMaterialShaderAttrib(ambient, 0);
    BindMaterialShaderAttrib(diffuse, 1);
    BindMaterialShaderAttrib(specular, 2);
    BindMaterialShaderAttrib(roughness, 3);
    BindMaterialShaderAttrib(metalness, 4);
    BindMaterialShaderAttrib(shininess, 5);
    BindMaterialShaderAttrib(normal, 6);
    if (tile.constant > 0)
    {
        BindOutsideShaderAttrib(tile);
    }
    /*
    if (tilable)
    {
       
        
    }*/
}

void BasicMaterial::FlushToShader(ShaderUniformsCache& cache)
{
    FlushToShader();
    cache.FlushToShader(m_IntrinsicShader);
}

void BasicMaterial::InitAttribs()
{
    /*
        MaterialAttrib ambient = "ambient";
    MaterialAttrib diffuse = "diffuse";
    MaterialAttrib specular = "specular";
    MaterialAttrib roughness = "roughness";
    MaterialAttrib metalness = "metalness";

    MaterialAttrib shininess = "shininess";
    // point light
    MaterialAttrib linear = "linear";
    MaterialAttrib constant = "constant";
    MaterialAttrib quadratic = "quadratic";
    */
    ambient = glm::vec3(1.0f);
    ambient.uniform = "ambient";

    diffuse = glm::vec3(1.0f);
    diffuse.uniform = "diffuse";

    specular = glm::vec3(.5f);
    specular.uniform = "specular";

    roughness = glm::vec3(.5f);
    roughness.uniform = "roughness";

    normal = glm::vec3(1.0f);
    normal.uniform = "normal";
    normal.reverseY = true;
    
    metalness = glm::vec3(0.0f);
    metalness.uniform = "metalness";

    shininess = .02f;
    shininess.uniform = "shininess";

    tile = 0.0f;
    tile.uniform = "tile";
    //InitDefaultSnippets();
}


void BasicMaterial::BindOutsideShaderAttrib(MaterialAttrib attrib) const
{
    if (attrib.type == MaterialAttribType::FLOAT)
    {
        m_IntrinsicShader->setUniform1f(std::string(attrib.uniform), attrib.constant);
    }
    if (attrib.type == MaterialAttribType::COLOR)
    {
        m_IntrinsicShader->setUniform3f(attrib.uniform, attrib.color.x, attrib.color.y, attrib.color.z);
    }
}

void BasicMaterial::SetTilable(bool t)
{
    if (t > 0)
    {
        if (!tilable) {
            AddShaderSnippet(ShaderSnippet::TilableSnippet(tile.uniform));
            tilable = true;
        }
    }
}

void BasicMaterial::SetTangent(bool hasNormal)
{
    if (hasNormal && normal.texture != nullptr)
    {
        AddShaderSnippet(ShaderSnippet::TangentSnippet(3, 4));
    }
}

void BasicMaterial::SetShadow(bool receiveShadow)
{
    if (receiveShadow)
    {
        AddShaderSnippet(ShaderSnippet::ShadowSnippet());
    }
}

void BasicMaterial::UpdateTextureFromPath(TextureType type, std::string path)
{
    if (path.empty())
    {
        return;
    }

    Texture* t = nullptr;
    if (type == TextureType::DIFFUSE_TEXTURE)
    {
        t = new Texture(path, true, true);
        diffuse.texture = t;
    }

    if (type == TextureType::METALNESS_TEXTURE)
    {
        t = new Texture(path, true, true);
        metalness.texture = t;
    }

    if (type == TextureType::NORMAL_TEXTURE)
    {
        t = new Texture(path, true, true);
        normal.texture = t;
        this->SetTangent(true);
    }

    if (type == TextureType::ROUGHNESS_TEXTURE)
    {
        t = new Texture(path, true, true);
        roughness.texture = t;
    }

    if (type == TextureType::SPECULAR_TEXTURE)
    {
        t = new Texture(path, true, true);
        specular.texture = t;
    }

}

void BasicMaterial::InitDefaultSnippets()
{
    SetShadow(true);
}

void BasicMaterial::UpdateTextureFromMemory(TextureType type, MemoryTextureData data)
{
    if (data.data != nullptr)
    {
        Texture *t = nullptr;
        if (type == TextureType::DIFFUSE_TEXTURE)
        {
            t = new Texture(data, true, true);
            diffuse.texture = t;
        }
        if (type == TextureType::METALNESS_TEXTURE)
        {
            t = new Texture(data, true, true);
            metalness.texture = t;
        }
        if (type == TextureType::NORMAL_TEXTURE)
        {
            t = new Texture(data, true, true);
            normal.texture = t;
            this->SetTangent(true);
        }

        if (type == TextureType::ROUGHNESS_TEXTURE)
        {
            t = new Texture(data, true, true);
            roughness.texture = t;
        }
        if (type == TextureType::SPECULAR_TEXTURE)
        {
            t = new Texture(data, true, true);
            specular.texture = t;
        }
    }
}