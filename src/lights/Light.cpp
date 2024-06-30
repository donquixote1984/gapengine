#include "Light.h"
#include "../shaders/ShaderType.h"
#include "../render/LightMaterial.h"
Light::Light():Geometry(GeometryType::LIGHT)
{}
Light::~Light()
{}

/*
Light::Light(const Light& light)
{
}
*/

void Light::SetColor(glm::vec3 color)
{
    //m_Mat->diffuse = color;
    //m_Color = color;
}
void Light::SetIntensity(float intensity)
{
    m_Intensity = intensity;
}

void Light::SetDirection(glm::vec3 direction)
{
    m_Direction = direction;
}

void Light::SetPosition(glm::vec3 position)
{
    m_Position = position;
}

glm::vec3 &Light::GetDirection()
{
    return m_Direction;
}

float * Light::GetPositionPtr()
{
    return &m_Position.x;
}

float * Light::GetDirectionPtr()
{
    return &m_Direction.x;
}

glm::vec3 Light::GetColor() const
{
    return m_Color;
}

float * Light::GetColorPtr()
{
    return &m_Color.x;
}

glm::vec3 & Light::GetPosition()
{
    return m_Position;
}

void Light::OnUpdateRender(GLFWwindow *window)
{
    Geometry::OnUpdateRender(window);
    OnUpdateColor();
}

void Light::OnUpdateColor()
{
  //  m_IntrinsicShader->Bind();
  //  m_IntrinsicShader->setUniform3f("light.diffuse", m_Mat.diffuse.x, m_Mat.diffuse.y, m_Mat.diffuse.z);
  //  m_IntrinsicShader->setUniform3f("light.position", m_Position.x, m_Position.y, m_Position.z); 
}

void Light::SetName(std::string &name)
{
    m_Name = name;
}

LightType Light::GetLightType() const
{
    return LightType::LIGHT_RAW;
}

void Light::GetImpact(LightImpact* impacts, int &size){}

unsigned int Light::GetIndex()
{
    return m_Index;
}
void Light::SetIndex(unsigned int index)
{
    m_Index = index;
}