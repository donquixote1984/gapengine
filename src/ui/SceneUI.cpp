#include "SceneUI.h"

using namespace settings;
SceneUI::SceneUI(GLFWwindow *window):UI(window)
{}
SceneUI::~SceneUI()
{}
void SceneUI::RenderLightModel()
{
	UISettings *settings = UISettings::GetSettings();
    ImGui::RadioButton("Blinn", &settings->lightMode, 0); ImGui::SameLine();
    ImGui::RadioButton("Phong", &settings->lightMode, 1); ImGui::SameLine();
	ImGui::NewLine();
}
void SceneUI::RenderShadow()
{
	UISettings *settings = UISettings::GetSettings();
	ImGui::Checkbox("Shadow", (bool *)&settings->shadow);
	ImGui::Checkbox("PCF", (bool *)&settings->PCF);
	ImGui::NewLine();
	ImGui::Text("CSM Layer");
	ImGui::SameLine(0.0f, 0.5f);
 	if (ImGui::ArrowButton("##left", ImGuiDir_Left)) { settings->csmlayer--; }
    ImGui::SameLine(0.0f, 0.1f);
    if (ImGui::ArrowButton("##right", ImGuiDir_Right)) { settings->csmlayer++; }
	ImGui::SameLine();
 	ImGui::Text("%d", settings->csmlayer);
}
void SceneUI::RenderNormal()
{
	UISettings * settings = UISettings::GetSettings();
	ImGui::Checkbox("Normal", (bool *)&settings->normal);
}

void SceneUI::RenderGamma()
{
	UISettings *settings = UISettings::GetSettings();

	if (ImGui::Checkbox("Gamma", (bool *)&settings->gamma))
	{
		//std::cout << "Enable Gamma" << std::endl;
		settings->ToggleGamma();
	} 
	ImGui::NewLine();
}

void SceneUI::RenderLights(const LightCollection * _lights) const
{
	UISettings *settings = UISettings::GetSettings();
	ImGui::Checkbox("Env light", (bool *)&settings->envlight);
	std::vector<Light*> lights = _lights->GetLights();
    for (int i = 0;  i < lights.size(); i++)
		{

			Light * light = lights[i];
			std::string lightName = light->GetName();
			ImGui::Separator();
			ImGui::Text("%s", lightName.c_str());
		//	ImGui::ColorEdit3(fmt::format("{} ambient color", lightName).c_str(), light->GetMaterialAmbientPtr());
		//	ImGui::ColorEdit3(fmt::format("{} Diffuse Color", lightName).c_str(), light->GetMaterialDiffsePtr());
		//	ImGui::ColorEdit3(fmt::format("{} Specular Color", lightName).c_str(), light->GetMaterialSpecularPtr());

			if (light->GetLightType() == LightType::DIRECTIONAL)
			{
   	     		ImGui::SliderFloat3(fmt::format("{} Direction", lightName).c_str(), light->GetDirectionPtr(), -5.0f, 5.0f);
			}
			else if (light -> GetLightType() == LightType::POINT)
			{
				ImGui::SliderFloat3(fmt::format("{} Position", lightName).c_str(), light->GetPositionPtr(), -10.0f, 10.0f);
			}
		}
}

void SceneUI::RenderStatistics()
{
	ImGui::Text("vertices: %d", UISettings::GetSettings()->verticesCount);
	ImGui::NewLine();
}

void SceneUI::RenderLoop()
{
	RenderFPS();
	RenderStatistics();
	RenderLightModel();
	RenderGamma();
	RenderShadow();
	RenderNormal();
	RenderLights(m_Lights);
	RenderSun();
}
void SceneUI::RenderSun()
{
	//ImGui::ShowDemoWindow();
	if (m_Sun != nullptr)
	{
		ImGui::SliderFloat3("Sun Direction", &m_Sun->x, -5.0f, 5.0f);
	}
}
void SceneUI::RegisterComponent(UIComponent component, void * ptr)
{
    switch (component)
    {
    case UIComponent::LIGHTS:
        m_Lights = static_cast<LightCollection*>(ptr);
        break;
	case UIComponent::SUN:
		m_Sun = static_cast<glm::vec3 *>(ptr);
    default:
        break;
    }
}
