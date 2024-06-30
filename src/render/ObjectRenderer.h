#pragma once

#include "RenderContext.h"
#include "GLFW/glfw3.h"
#include "../shaders/Shader.h"
#include "../shaders/GeometryShader.h"
#include "Material.h"

class ObjectRenderer
{
protected:
    //Shader *m_IntrinsicShader;
    //Material *m_Material;
    RenderContext rc;
public:
    void SetRC(RenderContext &rc);
};