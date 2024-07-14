#pragma once
#include <GLFW/glfw3.h>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ui/ImGuiUtil.h"
#include "global/ScreenProps.h"

// settings
const unsigned int SCR_WIDTH = 2400;
const unsigned int SCR_HEIGHT = 2160;

struct CameraSettings
{
    float nearPlane = .01f;
    float farPlane = 200.0f;
    float zoom = 45.0f;
};

struct CameraWorldCorner
{
    glm::vec4 corners[8];
  /*  glm::vec4 backLeftBottom;
    glm::vec4 backLeftTop;
    glm::vec4 backRightBottom;
    glm::vec4 backRightTop;

    glm::vec4 frontLeftBottom;
    glm::vec4 frontLeftTop;
    glm::vec4 frontRightBottom;
    glm::vec4 frontRightTop;
    */

    const glm::vec3 Center() const
    {
        //return (glm::vec3(backLeftBottom) + glm::vec3(backLeftTop) + glm::vec3(backRightBottom) + glm::vec3(backRightTop) + glm::vec3(frontLeftBottom) + glm::vec3(frontLeftTop) + glm::vec3(frontRightBottom) + glm::vec3(frontRightTop))/8.0f ;
        glm::vec4 c = glm::vec4(0.0f);
        for (int i = 0; i < 8; i++)
        {
            c += corners[i];
        }

        c /= 8;
        return c;
    }
    const glm::vec4 operator[](int x) const
    {
        if (x > 7) {
            return corners[0];
        }
        return corners[x];
    }
};

class Camera {
private:

    CameraSettings m_Settings;
    glm::vec3 m_Pos;
    glm::vec3 m_InitPos;
    glm::vec3 m_Translate = glm::vec3(0.0);
    glm::vec3 m_Offset = glm::vec3(0.0);
    glm::vec3 m_CamFront = glm::vec3(.0f, .0f, -1.0f);
    glm::vec3 m_CamUp = glm::vec3(.0f, 1.0f, .0f);
    glm::mat4 m_Projection = glm::perspective(glm::radians(m_Settings.zoom), (float)Global::screen.width/ (float)Global::screen.height, m_Settings.nearPlane, m_Settings.farPlane);

    bool m_FirstMouse = true;
    float m_LastMouseX = SCR_WIDTH / 2;
    float m_LastMouseY = SCR_HEIGHT / 2;
    float m_Yaw = -90.0f;
    float m_Pitch = 0.0f;
    bool m_YInversed = false;
    bool m_Naving = false;

    float m_NavingSpeed = 0.005f;
    float m_NavAngle = 0.0f;
public:
    Camera();
    Camera(glm::vec3 pos);
    ~Camera();
    void SetPosition(glm::vec3 pos);
    glm::vec3 GetPosition();
    void Bind(GLFWwindow *window);
    void onProcessInput(GLFWwindow *window);
    void onMouseMove(double xpos, double ypos);
    glm::mat4 GetView();
    glm::mat4 GetRight();
    glm::mat4 GetProjection();
    float GetPitch();
    float GetYaw();
    void InverseY(float height = 0);
    void ReverseY(float height = 0);
    void SetCamNav(bool);
    void ResetMouse();
    CameraSettings GetCameraSettings();
    static void cursorPosCallback(GLFWwindow* window, double xpos, double ypos)
    {
        Camera* camera = static_cast<Camera*>(glfwGetWindowUserPointer(window));

        if (ImGuiUtil::MouseInImGui()) 
        {
            return;
        }
        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)  
        {
            if (camera) {
                camera->onMouseMove(xpos, ypos);
            }
        } else 
        {
            if (camera) 
            {
                camera->ResetMouse();
            }
        }
    }
    void OnUpdate(GLFWwindow *window);
    CameraWorldCorner GetFrustumCornersWorldSpace();
    CameraWorldCorner GetFrustumCornersWorldSpace(float nearPlane, float farPlane);
    CameraWorldCorner GetFrustumCornersWorldSpace(glm::mat4 projection);
    
    glm::vec3 GetFrustumWorldCenter();
    void OnNav();
};
