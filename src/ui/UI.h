#pragma once

#include <GLFW/glfw3.h>
#include "imgui/imgui.h"
#include "ImGuiUtil.h"
#include "../lights/Light.h"
#include "../lights/LightCollection.h"
#include <fmt/format.h>
#include "UISettings.h"

enum class UIComponent {
    LIGHTS,
    SUN
};

class UI
{
private:
    ImGuiIO io;
public:
    UI(GLFWwindow *);
    virtual ~UI();
    void StartLoop();
    void EndLoop();
    void RenderFPS();
    virtual void RenderLoop() = 0;
    virtual void RegisterComponent(UIComponent, void*);
};