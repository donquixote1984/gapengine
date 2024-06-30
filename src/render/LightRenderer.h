#pragma once
#include "GLFW/glfw3.h"
#include "ObjectRenderer.h"


class LightRenderer: public ObjectRenderer
{
public:
    LightRenderer();
    void OnRender(GLFWwindow *window);
};