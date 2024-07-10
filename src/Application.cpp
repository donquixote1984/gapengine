#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <fmt/format.h>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "Camera.h"
#include "ui/UI.h"
#include "ui/SceneUI.h"
#include "Scene.h"
#include "JsonSceneReader.h"
#include "Ground.h"
#include "global/GlobalData.h"
#include "global/ScreenProps.h"
#include "Config.h"
#include "ApplicationInit.hpp"


#define ASSERT(x) if (!(x)) __builtin_debugtrap();
#define GLCALL(x) glClearError();\
	x;\
	ASSERT(glLogCall())

static void glClearError()
{
	while(glGetError() != GL_NO_ERROR);

}

#ifndef FRAMEBUFFER_CB
#define FRAMEBUFFER_CB
static void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}
#endif
static bool glLogCall()
{
	while(GLenum error = glGetError()) 
	{
		std::cout << "OpenGL Error: " << error << std::endl;
		return false;
	}
	return true;
}
class Application
{
public:
	Camera * camera = nullptr;
	Scene * scene = nullptr;
	SceneUI * ui = nullptr;

	Application(GLFWwindow *window)
	{
		camera = new Camera(glm::vec3(.0f, 1.0f, 8.0f));
		Global::camera = camera;

		scene = new Scene();
		Global::scene = scene;
		scene->AddCamera(camera);
		glfwSetWindowUserPointer(window, camera);
		glfwSetCursorPosCallback(window, Camera::cursorPosCallback);
		glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
		ui = new SceneUI(window);
		Global::ui = ui;

		JsonSceneReader j("./res/levels/demo/scene2/scene2.json");
		j.AddToScene(*scene);
		Ground *ground = new Ground();
		scene->AddGround(ground);
	}

	~Application()
	{
		delete camera;
		delete scene;
		delete ui;
	}
};

static Application * app;
static int GameInit(GLFWwindow *window)
{
	app = new Application(window);
	return 0;
}

static int GameLoop(GLFWwindow *window)
{
	app->ui->StartLoop();
	app->scene->OnUpdate(window);
	app->ui->RenderLoop();
	app->ui->EndLoop();
	return 0;
}
static int init()
{
    // glfw: initialize and configure
	// ------------------------------
	ApplicationInit::init(GameInit, GameLoop);
	return 0;
}

