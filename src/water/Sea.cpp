#include "Sea.h"
#include "../Scene.h"
#include "../global/ScreenProps.h"
#include "../global/GlobalData.h"
#include <GLFW/glfw3.h>
#include "../UniformBuffer.h"



Sea::Sea()
{
	m_SeaShader = new Shader("res/shaders/sea/sea.shader");
	m_SeaShader->Ready();
	m_SeaShader->BindToUniformBuffer(MatricesUniformBufferBindings::MatricesSlot);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
}

Sea::~Sea()
{
	delete m_SeaShader;
	m_SeaShader = nullptr;
}

void Sea::SetScene(Scene* scene)
{
	m_Scene = scene;
}


void Sea::OnRender()
{
	m_SeaShader->Bind();

	Camera* c = Global::camera;
	glm::vec3 pos = c->GetPosition();
	m_SeaShader->setUniform3f("camera", pos.x, pos.y, pos.z);

	m_SeaShader->setUniform2f("u_Resolution", (GLsizei)Global::screen.width, (GLsizei)Global::screen.height);
	m_SeaShader->setUniform1f("u_Time", static_cast<float>(glfwGetTime()));

	screenCanvas.Draw();
}
