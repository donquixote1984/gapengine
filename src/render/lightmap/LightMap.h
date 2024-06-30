#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include "../../shaders/Shader.h"
#include "../../ScreenCanvas.h"
#include "../../lights/Light.h"
#include "../../shaders/ShaderSnippet.h"

class Light;
class Geometry;
class LightMap 
{
protected:
    unsigned int m_LightMapFrameBufferObj;
    unsigned int m_LightMapTex;
    float m_ScreenWidth;
    float m_ScreenHeight;
    unsigned int m_Index;
    ScreenCanvas m_ScreenCanvas;
    Shader *m_LightMapShader = nullptr;
    Shader *m_LightMapDebugShader = nullptr;
    bool m_IsDebugging = false;
    virtual void InitShaders() = 0;
    float m_NearPlane= .01f;
    float m_FarPlane= 50.0f;
    float orthoUnit = 30.0f;
    glm::mat4 m_LightPerspectiveMat = glm::perspective(glm::radians(90.0f), 1.0f, m_NearPlane, m_FarPlane);
    glm::mat4 m_LightOrthoMat = glm::ortho(-orthoUnit, orthoUnit, -orthoUnit, orthoUnit, m_NearPlane, m_FarPlane);
    bool m_UseCSM = false;
public:
    LightMap(float width, float height);
    virtual ~LightMap();

    void EnableDebug();
    void Flush();
    void GetSize(float &width, float &height);
    void UnBind();
    bool IsDebugging();
    virtual void RenderDebug(Geometry*g, Light *light) = 0;
    virtual void RenderToGeometry(Geometry *g, Light *) = 0;
    virtual void OnRenderMVP(Geometry * geo, Light * light) = 0;
    virtual void OnRenderLightSpace(Light *) = 0;
    virtual void Bind() = 0;
    float GetFarPlane();
    void SetIndex(unsigned int);
    unsigned int GetIndex();
};