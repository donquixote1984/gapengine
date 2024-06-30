#pragma once
#include "ObjectRenderer.h"
#include "../geometry/Empty.h"
#include "RenderContext.h"
#include <vector>
class EmptyRenderer: public ObjectRenderer
{
protected:
    EmptyTransform *m_Empty;
    void OnRenderScript(GLFWwindow *window);
    void OnRenderMVP(GLFWwindow *window);
public:
    void OnRender(GLFWwindow *window);
    void OnRenderEmpties(std::vector<EmptyTransform *> &empties, GLFWwindow *window);
    EmptyRenderer();
    EmptyRenderer(RenderContext &rc);
};