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
public:
    EnvBox();
    virtual ~EnvBox();
    void OnUpdateMVP(RenderContext &rc);
    virtual void BindTexture(int slot = 0) = 0;
    virtual void ActiveEnvironment() = 0;
};