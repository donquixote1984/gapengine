#pragma once
#include "../shaders/Shader.h"
#include "../ScreenCanvas.h"

class Scene;
class Sea
{
private:
	Scene* m_Scene = nullptr;
	Shader* m_SeaShader = nullptr;
	ScreenCanvas screenCanvas;

public:
	Sea();
	~Sea();
	void SetScene(Scene* scene);
	void OnRender();
};