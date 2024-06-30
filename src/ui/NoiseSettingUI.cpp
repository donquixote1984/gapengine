#include "NoiseSettingUI.h"

NoiseSettingUI::NoiseSettingUI(GLFWwindow *window): UI(window)
{}
NoiseSettingUI::~NoiseSettingUI()
{}
 void NoiseSettingUI::RenderLoop()
 {
    //ImGui::ShowDemoWindow();
    ImGui::SliderInt("freq", &settings.freq, 1, 32);
    ImGui::SliderFloat("amp", &settings.amp, 1, 32);
    ImGui::SliderFloat2("offset", settings.offset, .0f, 16.0f);
    ImGui::SliderInt("octaves", &settings.octaves, 1, 10);
 }