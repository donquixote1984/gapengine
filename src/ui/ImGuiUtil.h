#pragma once

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

namespace ImGuiUtil
{

    ImGuiIO & init_imgui(GLFWwindow * window);
    void DrawImGuiBegin(ImGuiIO & io);
    void DrawImGuiEnd();
    void DrawImGuiDestroy();
    bool MouseInImGui();
}