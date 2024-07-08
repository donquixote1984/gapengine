#include "PointLight.h"
#include "../shaders/ShaderType.h"
#include "../render/LightMaterial.h"
#include "../render/PointLightMaterial.h"

double vertices[] = {
0.000000,-1.000000,0.000000,
0.723607,-0.447220,0.525725,
-0.276388,-0.447220,0.850649,
-0.894426,-0.447216,0.000000,
-0.276388,-0.447220,-0.850649,
0.723607,-0.447220,-0.525725,
0.276388,0.447220,0.850649,
-0.723607,0.447220,0.525725,
-0.723607,0.447220,-0.525725,
0.276388,0.447220,-0.850649,
0.894426,0.447216,0.000000,
0.000000,1.000000,0.000000,
-0.162456,-0.850654,0.499995,
0.425323,-0.850654,0.309011,
0.262869,-0.525738,0.809012,
0.850648,-0.525736,0.000000,
0.425323,-0.850654,-0.309011,
-0.525730,-0.850652,0.000000,
-0.688189,-0.525736,0.499997,
-0.162456,-0.850654,-0.499995,
-0.688189,-0.525736,-0.499997,
0.262869,-0.525738,-0.809012,
0.951058,0.000000,0.309013,
0.951058,0.000000,-0.309013,
0.000000,0.000000,1.000000,
0.587786,0.000000,0.809017,
-0.951058,0.000000,0.309013,
-0.587786,0.000000,0.809017,
-0.587786,0.000000,-0.809017,
-0.951058,0.000000,-0.309013,
0.587786,0.000000,-0.809017,
0.000000,0.000000,-1.000000,
0.688189,0.525736,0.499997,
-0.262869,0.525738,0.809012,
-0.850648,0.525736,0.000000,
-0.262869,0.525738,-0.809012,
0.688189,0.525736,-0.499997,
0.162456,0.850654,0.499995,
0.525730,0.850652,0.000000,
-0.425323,0.850654,0.309011,
-0.425323,0.850654,-0.309011,
0.162456,0.850654,-0.499995
};
PointLight::PointLight()//:Geometry(vertices, 42, {3})
{
    //InitGeometry("res/sphere.geo.data");
    //AttachShader(ShaderType::POINT_LIGHT_SHADER);
    //float _vertices = new float
    std::shared_ptr<float[]> vert = std::shared_ptr<float[]>(new float[sizeof(vertices)/ sizeof(float)]);
    float * v = vert.get();
    std::memcpy(v, vertices, sizeof(vertices));
    //InitGeometryData(vert, 42, {3});
    InitMaterial();
    //m_LightMap = new PointLightMap(Global::screen.width, Global::screen.height);
    //m_LightMap = new DirectionalLightMap(Global::screen.width, Global::screen.height);
    //m_LightMap->EnableDebug();
}
PointLight::~PointLight()
{
   // delete m_LightMap;
   // m_LightMap = NULL;
}
LightType PointLight::GetLightType() const
{
    return POINT;
}

void PointLight::InitMaterial()
{
    //m_Mat = new PointLightMaterial();
    //m_Mats.push_back(new PointLightMaterial());
}

void PointLight::GetImpact(LightImpact* impacts, int &size)
{
    //impacts.reserve(10);
    LightMaterial *lm = static_cast<LightMaterial *>(m_Mats[0]);
    const glm::vec3 &amb = lm->GetAmbient();
    const glm::vec3 &diff = lm->GetDiffuse();
    float constant = lm->GetConstant();
    float quad = lm->GetQuadratic();
    float linear = lm->GetLinear();
    const glm::vec3 &spec = lm->GetSpecular();

    impacts[0] = {LightImpactKey::LIPT_POSITION, 3, {m_Position.x, m_Position.y, m_Position.z}};
    impacts[1] = {LightImpactKey::LIPT_AMBIENT, 3, {amb.r, amb.g, amb.b}};
    impacts[2] = {LightImpactKey::LIPT_CONSTANT, 1, {constant}};
    impacts[3] = {LightImpactKey::LIPT_QUADRATIC, 1, {quad}};
    impacts[4] = {LightImpactKey::LIPT_LINEAR, 1, {linear}};
    impacts[5] = {LightImpactKey::LIPT_DIFFUSE, 3, {diff.r, diff.g, diff.b}};
    impacts[6] = {LightImpactKey::LIPT_SPECUAR, 3, {spec.r, spec.g, spec.b}};

    size = 7;
}

float PointLight::GetFarPlane()
{
    LightMaterial *lm = static_cast<LightMaterial *> (m_Mats[0]);
    return glm::length(lm->GetDiffuse());
}

std::string PointLight::GetName()
{
    return "PointLight " + std::to_string(GetIndex());
}
ShaderPointLight PointLight::Serialize()
{

    LightMaterial *lm = static_cast<LightMaterial *>(m_Mats[0]);
    ShaderPointLight spl;
    spl.castShadow = 1;
    spl.intensity = lm->GetIntensity();
    spl.constant = lm->GetConstant();
    spl.diffuse = {lm->GetDiffuse(), .0f};
    spl.index = this->GetIndex();
    spl.farPlane = this->GetFarPlane();
    spl.linear = lm->GetLinear();
    spl.quadratic = lm->GetQuadratic();
    spl.specular = {lm->GetSpecular(), .0f};
    spl.ambient = {lm->GetAmbient(), .0f};
    spl.position = {m_Position, .0f};
    return spl;
}