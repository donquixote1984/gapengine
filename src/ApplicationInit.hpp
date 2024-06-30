#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "ui/UI.h"
#include "lights/PointLight.h"
#include "FrameBuffer.h"
#include "global/GlobalData.h"
#include "Logger.h"
#include "global/ScreenProps.h"
#include "Config.h"


class ApplicationInit
{
public:
    static int init(int (*gameinit)(GLFWwindow *window), int (*gameloop)(GLFWwindow *window))
    {
            // glfw: initialize and configure
        // ------------------------------
        float deltaTime = 0.0f;
        float lastFrame = 0.0f;
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
        glfwWindowHint(GLFW_SAMPLES, 4);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    #ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); //uncomment this statement to fix compilation on OS X
    #endif
        // glfw window creation
        // --------------------

        
        GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "", NULL, NULL);
        glfwMakeContextCurrent(window);

        //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        if (window == NULL)
        {
            std::cout << "Failed to create GLFW window" << std::endl;
            glfwTerminate();
            return -1;
        }
        // glad: load all OpenGL function pointers
        // ---------------------------------------
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            std::cout << "Failed to initialize GLAD" << std::endl;
            return -1;
        }

        std::cout << glGetString(GL_VERSION) << std::endl;

        glEnable(GL_MULTISAMPLE);
        glEnable(GL_DEPTH_TEST);

        glDepthFunc(GL_LESS);
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);

        glEnable(GL_STENCIL_TEST);

        GLint vp[4];
        glGetIntegerv(GL_VIEWPORT, vp);
        float realScreenWidth = (float)vp[2];
        float realScreenHeight = (float)vp[3];
        float dpr = realScreenWidth / SCR_WIDTH;
        Global::SetScreenSize(realScreenWidth, realScreenHeight, dpr);
        ConfigIniReader::ReadConfigIni();
        gameinit(window);
        std::srand(unsigned(std::time(nullptr)));
        glViewport(0, 0, (GLsizei)realScreenWidth, (GLsizei)realScreenHeight);

        

        while (!glfwWindowShouldClose(window))
        {
            float currentFrame = static_cast<float>(glfwGetTime());
            deltaTime = currentFrame - lastFrame;
            lastFrame = currentFrame;
            //glClearColor(0, 0, 0, 1);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
            // main loop
            gameloop(window);
            // 

            glfwSwapBuffers(window);
		    glfwPollEvents();
        }

        Global::FreeGlobalData();
        glfwTerminate();
        ImGuiUtil::DrawImGuiDestroy();
        return 0;
    }
};