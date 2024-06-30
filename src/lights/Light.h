#pragma once
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "../geometry/Geometry.h"
#include "../shaderstructures/ShaderStructures.hpp"
#include "../render/lightmap/LightMap.h"

enum LightType {
    LIGHT_RAW, 
    POINT, 
    DIRECTIONAL,
    SUN
};

enum LightStatus {LIGHT_ON, LIGHT_OFF};

enum LightImpactKey
{
    LIPT_POSITION,
    LIPT_AMBIENT,
    LIPT_DIFFUSE,
    LIPT_CONSTANT,
    LIPT_LINEAR,
    LIPT_QUADRATIC,
    LIPT_DIRECTION,
    LIPT_SPECUAR,
    LIPT_METALNESS
};

struct LightImpact
{
    LightImpactKey key;
    int stride;
    float data[3];
};

class LightMap;
class Light : public Geometry
{
private:

    float m_Intensity=1.0f;
    unsigned int m_Index;
    std::string m_Name;
    LightStatus lightStatus = LightStatus::LIGHT_ON;
protected:
    //LightMap * m_LightMapCSM = nullptr;

    glm::vec3 m_Color = glm::vec3(1.0f);
    glm::vec3 m_Direction = glm::vec3(-1.0f, -1.0f, -1.0f);

public:
    //Light(LightType lightType);
    //Light(const Light &light);
    Light();
    virtual ~Light();
    void SetColor(glm::vec3);
    glm::vec3 GetColor() const;
    float * GetColorPtr();
    void SetPosition(glm::vec3);
    glm::vec3 & GetPosition();
    void SetDirection(glm::vec3);
    glm::vec3 & GetDirection();
    float * GetDirectionPtr();
    float * GetPositionPtr();
    virtual float GetFarPlane() = 0;
    void SetIntensity(float intensity);
    void OnUpdateRender(GLFWwindow *window);
    virtual LightType GetLightType() const;
    virtual std::string GetName() = 0;
    void SetName(std::string &name);
    void OnUpdateColor();
    virtual void GetImpact(LightImpact* impacts, int &size);
   

    unsigned int GetIndex();
    void SetIndex(unsigned int index);
    
};