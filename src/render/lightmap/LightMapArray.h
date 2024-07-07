#pragma once
#include <glad/glad.h>
#include "GLFW/glfw3.h"
#include "../../Config.h"
#include "../../geometry/Geometry.h"
#include "../../global/ScreenProps.h"
#include "../../shaders/Shader.h"
#include "../../ScreenCanvas.h"
#include "../../shaders/ShaderSnippet.h"
#include "../../lights/LightCollection.h"
#include "../RenderContext.h"


class LightMapRenderer;
class LightMapArray
{
    friend class LightMapRenderer;
protected:
    unsigned int m_LightMapArrayFrameBufferObj;
    unsigned int m_LightMapArrayTex;
    unsigned int m_Res = 0;
    unsigned int m_Size = 0;

    float m_ScreenWidth = Global::screen.width;
    float m_ScreenHeight = Global::screen.height;

    LightCollection *m_Lights = nullptr;

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
    LightMapArray(unsigned int textureType, unsigned int res, unsigned int size);
    virtual ~LightMapArray();
    virtual void Bind() = 0;
    void UnBind();
    virtual void ActiveTexture() = 0;
    virtual void OnRenderLightSpace() = 0;
    virtual void OnRenderMVP(Geometry * geo, RenderContext &rc);

    bool IsDebugging();
    void EnableDebug();

    virtual void RenderDebug(Geometry*g) = 0;
    virtual void OnRenderGeometry(Geometry * g, RenderContext &rc);
    virtual bool HasLights() = 0;
};