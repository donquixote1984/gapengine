#pragma once
#include "Shader.h"
#include "ShaderBatch.h"
#include "ShaderType.h"
#include "../exceptions/InvalidTypeException.h"
#include "../UniformBuffer.h"
#include "../global/ShaderPartials.h"
#include "GeometryShaderAddOn.h"
#include <iostream>
class GeometryShaderPartials {
public:


    // vertex

    inline static const std::string VERT_SHADOW_H = "vertex/shadow.h.shader";
    inline static const std::string VERT_UNIFORM_H = "vertex/uniforms.h.shader";

    // fragment
    inline static const std::string FRAG_SHADOW_H = "fragment/shadow.function.h.shader";
    inline static const std::string FRAG_DEFINE_H = "fragment/define.h.shader";
    inline static const std::string FRAG_IO_H = "fragment/io.h.shader";
    inline static const std::string FRAG_PLIGHT_FUNC_H = "fragment/shadow.point.function.h.shader";
    inline static const std::string FRAG_DLIGHT_FUNC_H = "fragment/shadow.directional.function.h.shader";
    inline static const std::string FRAG_LIGHT_FUNC_H = "fragment/light.function.h.shader";
    inline static const std::string FRAG_SETTINGS_H = "fragment/settings.h.shader";
    inline static const std::string FRAG_LIGHT_STRUCT_H = "fragment/light.struct.h.shader";
    inline static const std::string FRAG_MAT_FUNC_H = "fragment/material.function.h.shader";
    inline static const std::string FRAG_MAT_STRUCT_H = "fragment/material.struct.h.shader";
    inline static const std::string FRAG_NORMAL = "fragment/normal.shader";
    inline static const std::string FRAG_UNIFORMS_H = "fragment/uniforms.h.shader";
    inline static const std::string FRAG_CSM_H = "fragment/csm.h.shader";
    inline static const std::string FRAG_PBR_H = "fragment/pbr.function.h.shader";
    inline static const std::string FRAG_LIGHT_PBR_H = "fragment/light.pbr.function.h.shader";

    static void AddPartials(Shader *shader)
    {
        shader->AddPartials(GeometryShaderPartials::VERT_UNIFORM_H, Global::GetShaderPartials(GeometryShaderPartials::VERT_UNIFORM_H));
        shader->AddPartials(GeometryShaderPartials::FRAG_DEFINE_H, Global::GetShaderPartials(GeometryShaderPartials::FRAG_DEFINE_H));
        shader->AddPartials(GeometryShaderPartials::FRAG_IO_H, Global::GetShaderPartials(GeometryShaderPartials::FRAG_IO_H));
        shader->AddPartials(GeometryShaderPartials::FRAG_PLIGHT_FUNC_H, Global::GetShaderPartials(GeometryShaderPartials::FRAG_PLIGHT_FUNC_H));
        shader->AddPartials(GeometryShaderPartials::FRAG_DLIGHT_FUNC_H, Global::GetShaderPartials(GeometryShaderPartials::FRAG_DLIGHT_FUNC_H));
        shader->AddPartials(GeometryShaderPartials::FRAG_LIGHT_FUNC_H, Global::GetShaderPartials(GeometryShaderPartials::FRAG_LIGHT_FUNC_H));
        shader->AddPartials(GeometryShaderPartials::FRAG_LIGHT_STRUCT_H, Global::GetShaderPartials(GeometryShaderPartials::FRAG_LIGHT_STRUCT_H));
        shader->AddPartials(GeometryShaderPartials::FRAG_MAT_FUNC_H, Global::GetShaderPartials(GeometryShaderPartials::FRAG_MAT_FUNC_H));
        shader->AddPartials(GeometryShaderPartials::FRAG_MAT_STRUCT_H, Global::GetShaderPartials(GeometryShaderPartials::FRAG_MAT_STRUCT_H));
        shader->AddPartials(GeometryShaderPartials::FRAG_NORMAL, Global::GetShaderPartials(GeometryShaderPartials::FRAG_NORMAL));
        shader->AddPartials(GeometryShaderPartials::FRAG_UNIFORMS_H, Global::GetShaderPartials(GeometryShaderPartials::FRAG_UNIFORMS_H));
        shader->AddPartials(GeometryShaderPartials::FRAG_SETTINGS_H, Global::GetShaderPartials(GeometryShaderPartials::FRAG_SETTINGS_H));
        shader->AddPartials(GeometryShaderPartials::FRAG_SHADOW_H, Global::GetShaderPartials(GeometryShaderPartials::FRAG_SHADOW_H));
        shader->AddPartials(GeometryShaderPartials::FRAG_CSM_H, Global::GetShaderPartials(GeometryShaderPartials::FRAG_CSM_H));
    }
    static void AddPBR(Shader * shader)
    {
        shader->AddPartials(GeometryShaderPartials::FRAG_PBR_H, Global::GetShaderPartials(GeometryShaderPartials::FRAG_PBR_H));
        shader->AddPartials(GeometryShaderPartials::FRAG_LIGHT_PBR_H, Global::GetShaderPartials(GeometryShaderPartials::FRAG_LIGHT_PBR_H));
    }
};

class GeometryShader: public ShaderBatch
{
public:
    GeometryShader(const std::string shaderPath): ShaderBatch(shaderPath)
    {}
    //virtual ShaderType GetShaderType() { return ShaderType::NO_SHADER;}

    void EnableAddOn(GeometryShaderAddOn *g)
    {
        SourceType type;
        std::string shaderSource = g->GetShaderSource(type);
        if (type == SourceType::FILE) 
        {
            this->AddFile(shaderSource);
        }

        if (type == SourceType::SOURCE)
        {
            this->AddSource(shaderSource);
        }
        if (!shaderSource.empty())
        {
            g->ApplySnippets(this);
        }
    }
};

class BasicGeometryShader: public GeometryShader
{
public:
    BasicGeometryShader(): GeometryShader("./res/shaders/geometry/geometry.shader")
    {}
    ShaderType GetShaderType() override
    {
        return ShaderType::BASIC_SHADER;
    }
};


class PointShader: public GeometryShader
{
public:
    PointShader() : GeometryShader("./res/shaders/geometry/point.shader")
    {}
    ShaderType GetShaderType() override
    {
        return ShaderType::POINT_SHADER;
    }
};

class PointLightShader: public GeometryShader
{
public:
    PointLightShader(): GeometryShader("./res/shaders/light/light.shader")
    {}
    ShaderType GetShaderType() override
    {
        return ShaderType::POINT_LIGHT_SHADER;
    }
};
class DirectionalLightShader: public GeometryShader
{
public: 
    DirectionalLightShader(): GeometryShader("./res/shaders/light/light.directional.shader")
    {}
    ShaderType GetShaderType() override
    {
        return ShaderType::DIRECTIONAL_LIGHT_SHADER;
    }
};

class DefaultGeometryShader: public GeometryShader
{
private:
public:
    DefaultGeometryShader(): GeometryShader( "./res/shaders/geometry/geometry.v2.shader")
    {
        //UniformBuffer * ub = UniformBuffer::GetUniformBuffer();
        //ub->BindShaderSlot(m_RendererID, "Matrices");
        // vert
        //this->AddPartials(GeometryShaderPartials::VERT_SHADOW_H, Global::GetShaderPartials(GeometryShaderPartials::VERT_SHADOW_H));
    

        // frag
        GeometryShaderPartials::AddPartials(this);
        //GeometryShaderPartials::FRAG_DEFINE_H
        //this->AddPartials
    }
    void InitDefaultBindings() override
    {
        this->Bind();
        this->setUniform1i("u_CascadeShadowMap", constants::CSM);
        this->setUniform1i("u_CascadeShadowMapSun", constants::SUN_CSM);
        this->setUniform1i("u_ShadowCubes", constants::SHADOWCUBE);
        this->setUniform1i("u_IRMap", constants::IR_MAP);
        this->setUniform1i("u_PrefilterMap", constants::PREFILTER_MAP);
        this->setUniform1i("u_BRDFLut", constants::BRDFLUT_MAP);
        this->setUniform1i("u_UseCSM", 1);
        //Atomsphere LUT
  
        this->setUniform1i("transmittance_texture", constants::TRANSMITTANCE_TEXTURE);
        this->setUniform1i("scattering_texture", constants::SCATERING_TEXTURE);
        this->setUniform1i("irradiance_texture", constants::IRRADIANCE_TEXTURE);
        this->setUniform1i("single_mie_scattering_texture", constants::SINGLE_MIE_SCATTERING_TEXTURE);

        this->UnBind();
    }
    ShaderType GetShaderType() override
    {
        return ShaderType::DEFAULT_SHADER;
    }
};

class PBRGeometryShader: public GeometryShader
{
public:
    PBRGeometryShader() : GeometryShader("./res/shaders/geometry/geometry.pbr.shader")
    {
        GeometryShaderPartials::AddPartials(this);
        GeometryShaderPartials::AddPBR(this);
        EnableAddOn(AtomsphereShaderAddOn::get());
    }
    void InitDefaultBindings() override
    {
        this->Bind();
        this->setUniform1i("u_UseCSM", 1);
        this->setUniform1i("u_CascadeShadowMap", constants::CSM);
        this->setUniform1i("u_CascadeShadowMapSun", constants::SUN_CSM);
        this->setUniform1i("u_ShadowCubes", constants::SHADOWCUBE);
        this->setUniform1i("u_IRMap", constants::IR_MAP);
        this->setUniform1i("u_PrefilterMap", constants::PREFILTER_MAP);
        this->setUniform1i("u_BRDFLut", constants::BRDFLUT_MAP);

        this->setUniform1i("transmittance_texture", constants::TRANSMITTANCE_TEXTURE);
        this->setUniform1i("scattering_texture", constants::SCATERING_TEXTURE);
        this->setUniform1i("irradiance_texture", constants::IRRADIANCE_TEXTURE);
        this->setUniform1i("single_mie_scattering_texture", constants::SINGLE_MIE_SCATTERING_TEXTURE);

        this->UnBind();
    }
    ShaderType GetShaderType() override
    {
        return ShaderType::PBR_SHADER;
    }
};
class TerrainShader: public GeometryShader
{
public:
    TerrainShader() : GeometryShader("./res/shaders/geometry/geometry.terrain.shader")
    {
        GeometryShaderPartials::AddPartials(this);
        GeometryShaderPartials::AddPBR(this);
        EnableAddOn(AtomsphereShaderAddOn::get());
    }

      void InitDefaultBindings() override
    {
        this->Bind();
        this->setUniform1i("u_CascadeShadowMap", constants::CSM);
        this->setUniform1i("u_CascadeShadowMapSun", constants::SUN_CSM);
        this->setUniform1i("u_UseCSM", 1);
        this->setUniform1i("u_ShadowCubes", constants::SHADOWCUBE);
        this->setUniform1i("u_IRMap", constants::IR_MAP);
        this->setUniform1i("u_PrefilterMap", constants::PREFILTER_MAP);
        this->setUniform1i("u_BRDFLut", constants::BRDFLUT_MAP);

        this->setUniform1i("transmittance_texture", constants::TRANSMITTANCE_TEXTURE);
        this->setUniform1i("scattering_texture", constants::SCATERING_TEXTURE);
        this->setUniform1i("irradiance_texture", constants::IRRADIANCE_TEXTURE);
        this->setUniform1i("single_mie_scattering_texture", constants::SINGLE_MIE_SCATTERING_TEXTURE);

        this->UnBind();
    }
    ShaderType GetShaderType() override
    {
        return ShaderType::TERRAIN_SHADER;
    }
};

class ReflectiveGeometryShader: public GeometryShader
{
public:
    ReflectiveGeometryShader(): GeometryShader("./res/shaders/geometry/reflective.shader")
    {
        //UniformBuffer * ub = UniformBuffer::GetUniformBuffer();
        //ub->BindShaderSlot(m_RendererID, "Matrices");
    }
    ShaderType GetShaderType() override
    {
        return ShaderType::REFLECTIVE_SHADER;
    }
};

class WaterShader: public GeometryShader
{
public:
    WaterShader(): GeometryShader("../res/shaders/geometry/water.shader")
    {}

    void InitDefaultBindings() override
    {
        this->Bind();
        this->setUniform1i("u_ReflectionTexture", constants::REFLECTION_TEXTURE);
        this->setUniform1i("u_RefractionTexture", constants::REFRACTION_TEXTURE);
        this->setUniform1i("u_FlowTexture", constants::FLOW_TEXTURE);

        this->setUniform1i("u_IRMap", constants::IR_MAP);
        this->setUniform1i("u_PrefilterMap", constants::PREFILTER_MAP);
        this->setUniform1i("u_BRDFLut", constants::BRDFLUT_MAP);

        this->setUniform1i("transmittance_texture", constants::TRANSMITTANCE_TEXTURE);
        this->setUniform1i("scattering_texture", constants::SCATERING_TEXTURE);
        this->setUniform1i("irradiance_texture", constants::IRRADIANCE_TEXTURE);
        this->setUniform1i("single_mie_scattering_texture", constants::SINGLE_MIE_SCATTERING_TEXTURE);
        this->UnBind();
    }
    ShaderType GetShaderType() override
    {
        return ShaderType::WATER;
    }
};

class VolumeShader: public GeometryShader
{
public:
    VolumeShader(): GeometryShader("../res/shaders/volume/volume.shader")
    {}
    void InitDefaultBindings() override
    {}
    ShaderType GetShaderType() override
    {
        return ShaderType::VOLUME;
    }
};

class SimpleCloudShader: public GeometryShader
{
public:
    SimpleCloudShader(): GeometryShader("../res/shaders/volume/simplecloud.shader")
    {}
    ShaderType GetShaderType() override
    {
        return ShaderType::SIMPLE_CLOUD;
    }
};

class CloudShader: public GeometryShader
{
public:
    CloudShader(): GeometryShader("../res/shaders/volume/cloud.shader")
    {}
    ShaderType GetShaderType() override
    {
        return ShaderType::CLOUD;
    }
};

class TerrainSDFShader: public GeometryShader
{
public:
    TerrainSDFShader(): GeometryShader("../res/shaders/volume/raymarchterrain.shader")
    {}
    ShaderType GetShaderType() override
    {
        return ShaderType::RAYMARCH_TERRAIN;
    }
};

class GeometryShaderFactory
{
public:
    static GeometryShader * CreateGeometryShader(ShaderType st)
    {
        if (st == ShaderType::BASIC_SHADER)
        {
            return new BasicGeometryShader();
        }

        if (st == ShaderType::DEFAULT_SHADER)
        {
            return new DefaultGeometryShader();
        }
        if (st == ShaderType::PBR_SHADER)
        {
            return new PBRGeometryShader();
        }
        if (st == ShaderType::TERRAIN_SHADER)
        {
            return new TerrainShader();
        }
        if (st == ShaderType::POINT_LIGHT_SHADER)
        {
            return new PointLightShader();
        }
        if (st == ShaderType::DIRECTIONAL_LIGHT_SHADER)
        {
            return new DirectionalLightShader();
        }

        if (st == ShaderType::REFLECTIVE_SHADER)
        {
            return new ReflectiveGeometryShader();
        }
        if (st == ShaderType::POINT_SHADER)
        {
            return new PointShader();
        }

        if (st == ShaderType::WATER)
        {
            return new WaterShader();
        }
        if (st == ShaderType::VOLUME)
        {
            return new VolumeShader();
        }
        if (st == ShaderType::SIMPLE_CLOUD)
        {
            return new SimpleCloudShader();
        }

        if (st == ShaderType::RAYMARCH_TERRAIN)
        {
            return new TerrainSDFShader();
        }
        if (st == ShaderType::CLOUD)
        {
            return new CloudShader();
        }

        std::string exp = "Invalid Geometry Shader";
        throw InvalidTypeException(exp.c_str());
    }
};