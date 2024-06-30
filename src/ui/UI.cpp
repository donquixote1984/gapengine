#include "./UI.h"

using namespace settings;
UI::UI(GLFWwindow *window)
{
    io = ImGuiUtil::init_imgui(window);
}
UI::~UI()
{}
void UI::StartLoop()
{
    ImGuiUtil::DrawImGuiBegin(io);
}
void UI::EndLoop()
{
    ImGuiUtil::DrawImGuiEnd();
}

void UI::RenderFPS()
{
	float fps = ImGui::GetIO().Framerate;
	ImGui::Text("%f", fps);
	ImGui::NewLine();
}
void UI::RegisterComponent(UIComponent c, void* ptr)
{}