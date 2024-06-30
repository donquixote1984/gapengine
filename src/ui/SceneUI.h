#pragma once
#include "UI.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
class SceneUI: public UI
{
private:
    LightCollection * m_Lights = nullptr;
    glm::vec3 * m_Sun = nullptr;
public:
    SceneUI(GLFWwindow *window);
    ~SceneUI();
    void RenderLights(const LightCollection * lights) const;
    void RenderLightModel();
    void RenderGamma();
    void RenderShadow();
    void RenderNormal();
    void RenderStatistics();
    void RenderPostProcess();
    void RenderLoop() override;
    void RegisterComponent(UIComponent ui, void *) override;
    void RenderSun();
};
