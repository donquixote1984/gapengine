#include "GeometryInstancing.h"

GeometryInstancing::GeometryInstancing(GeometryType gType): Geometry(gType)
{}
GeometryInstancing::GeometryInstancing(GeometryType gtype, GeometryInstancingType itype): Geometry(gtype), m_InstancingType(itype)
{}


void GeometryInstancing::SetInstancing(std::shared_ptr<Transform[]> mvps, unsigned int count)
{
    m_InstancingMVP = mvps;
    m_InstancingCount = count;
}

GeometryInstancingType GeometryInstancing::GetInstancingType()
{
    return m_InstancingType;
}

int GeometryInstancing::GetInstancingCount()
{
    return m_InstancingCount;
}

Transform * GeometryInstancing::GetInstancingMVP()
{
    return m_InstancingMVP.get();
}

void GeometryInstancing::ResolveMeta()
{
}
bool GeometryInstancing::IsInstancing()
{
    return true;
}

void GeometryInstancing::InitGeoData()
{
    //m_Gdp = new GeometryInstancingDataProcessor();
}
void GeometryInstancing::InitProcessor()
{
    m_Dp = new GeometryInstancingDataProcessor();
}

void GeometryInstancing::SetMaterial(Material *m)
{
    if (m_Mat != nullptr)
    {
        delete m_Mat;
    }
    m_Mat = m;
}
void GeometryInstancing::SetInstancingCount(int count)
{
    m_InstancingCount = count;
}

void GeometryInstancing::ReadyToScene()
{
    int offset = m_GeoData->GetLayoutSize();
    m_Mat->AddShaderSnippet(ShaderSnippet::InstancingSnippet(offset));
    Shader * shader = m_Mat->GetShader();
    //UniformBuffer* ubo = UniformBuffer::GetUniformBuffer();
    //shader->BindToUniformBuffer(ubo, "Matrices");
    shader->Ready();
    shader->Bind();
    shader->setUniform1i("instancingCount", m_InstancingCount);
    shader->BindToUniformBuffer(MatricesUniformBufferBindings::MatricesSlot);
    shader->BindToUniformBuffer(MatricesUniformBufferBindings::CSMSlot);
    shader->BindToUniformBuffer(LightUniformBufferBindings::LightSlot);
    shader->BindToUniformBuffer(SettingsUniformBufferBindings::SettingsSlot);
    shader->BindToUniformBuffer(MatricesUniformBufferBindings::PSMSlot);
    shader->BindToUniformBuffer(MatricesUniformBufferBindings::SUNSlot);
    //shader->BindToUniformBuffer(UniformBufferBindings::TEST_MVP_1);
    //shader->BindToUniformBuffer(UniformBufferBindings::TEST_MVP_2);
}