#pragma once
#include "../geometry/RawVertices.h"
#include "../render/RenderContext.h"
#include "../util/CommonData.h"
#include "../shaders/Shader.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class EnvBox: public RawVertices
{
protected:
    Shader *m_Shader = nullptr;
    bool m_HideBackground = false;
    bool m_Blur = false;
    float m_Exposure = 1.0f;
public:
    EnvBox();
    virtual ~EnvBox();
    void OnUpdateMVP(RenderContext &rc);
    virtual void BindTexture(int slot = 0) = 0;
    virtual void ActiveEnvironment() = 0;
    void HideBackground(bool hide);
    void SetExposure(float exposure);
    void SetBlur(bool blur);
    float GetExposure();
};