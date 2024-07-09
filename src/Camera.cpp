#include <algorithm> 
#include <iostream>
#include "Camera.h"


Camera::Camera()
{

}
Camera::Camera(glm::vec3 pos): m_Pos(pos), m_InitPos(pos)
{
	m_Offset.y = pos.y;
	
}

Camera::~Camera() 
{
}
void Camera::Bind(GLFWwindow *window)
{

}

void Camera::onProcessInput(GLFWwindow *window) 
{
    float cameraSpeed = .05f;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		m_Pos 	   += cameraSpeed * m_CamFront;
		//m_Offset.z += cameraSpeed * m_CamFront.z;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		m_Pos 	 -= cameraSpeed * glm::cross(m_CamFront, m_CamUp);
		m_Offset -= cameraSpeed * glm::cross(m_CamFront, m_CamUp);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		m_Pos -= cameraSpeed * m_CamFront;
		m_Translate = m_Pos - m_InitPos;
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		m_Pos    += cameraSpeed * glm::cross(m_CamFront, m_CamUp);
		m_Offset += cameraSpeed * glm::cross(m_CamFront, m_CamUp);
	}	

	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
		m_Pos    += cameraSpeed * m_CamUp;
		//m_Offset += cameraSpeed * glm::cross(m_CamFront, m_CamUp);
		m_Offset.y += cameraSpeed * m_CamUp.y;
	}	

	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
		m_Pos    -= cameraSpeed * m_CamUp;
		m_Offset.y -= cameraSpeed * m_CamUp.y;
	}	
}
void Camera::onMouseMove(double x, double y)
{
	if (m_FirstMouse) {
		m_LastMouseX = x ;
		m_LastMouseY = y ;
		m_FirstMouse = false;
		//m_Offset = m_Pos;

		//m_Pos = glm::length(m_Pos - m_Offset) * -m_CamFront + m_Offset + m_InitPos;
		return;
	}

	float xOffset = x - m_LastMouseX;
	float yOffset = m_LastMouseY - y;

	m_LastMouseX = x;
	m_LastMouseY = y;

	float sensitivity = 0.2f;
	xOffset *= sensitivity;
    yOffset *= sensitivity;

	m_Yaw += xOffset;
	m_Pitch += yOffset;

	m_Pitch = std::clamp(m_Pitch, -89.0f, 89.0f);
	//m_Yaw = std::clamp(m_Yaw, -89.0f, 89.0f);

	glm::vec3 front;
	front.x = cos(glm::radians(m_Yaw))*cos(glm::radians(m_Pitch));
	front.y = sin(glm::radians(m_Pitch)); 
	front.z = sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
	m_CamFront = glm::normalize(front);
	m_Pos = glm::length(m_Pos - m_Offset) * -m_CamFront + m_Offset ;
}

glm::mat4 Camera::GetView() 
{
	return glm::lookAt(m_Pos, m_Pos+ m_CamFront, m_CamUp);
}
glm::mat4 Camera::GetRight()
{
	glm::vec3 right = glm::cross(glm::vec3(0, 1, 0), m_CamFront);
	right = glm::cross(right, m_CamFront);

	//view_ray = cross(cross(view_ray, vec3(0,1,0)), view_ray);
	return glm::lookAt(m_Pos, m_Pos+ m_CamFront, glm::vec3(0, -1, 0));
}

glm::mat4 Camera::GetProjection()
{
	return m_Projection;
}
/*
void Camera::AddToScene(Scene *scene)
{
	m_Scene = scene;
	m_Scene->Bind();
	Shader *shader = m_Scene->GetShader();
	shader->setUniform4m("projection", m_Projection);
}
*/

void Camera::OnUpdate(GLFWwindow *window)
{
	onProcessInput(window);
	//m_Scene->Bind();
	//Shader *shader = m_Scene->GetShader();	
	//shader->setUniform4m("projection", m_Projection);
	//shader->setUniform4m("view", getView());
	//shader->setUniform3f("camPos", m_Pos.x, m_Pos.y, m_Pos.z);
	OnNav();
}

void Camera::OnNav()
{

}

void Camera::SetPosition(glm::vec3 pos)
{
	m_Pos = pos;
}

glm::vec3 Camera::GetPosition()
{
	return m_Pos;
}
void Camera::ResetMouse()
{
	m_FirstMouse = true;
}

CameraSettings Camera::GetCameraSettings()
{
	return m_Settings;
}

CameraWorldCorner Camera::GetFrustumCornersWorldSpace()
{
	return GetFrustumCornersWorldSpace(m_Projection);
}
CameraWorldCorner Camera::GetFrustumCornersWorldSpace(glm::mat4 projection)
{
	CameraWorldCorner cwc;
	const auto inv = glm::inverse(projection * GetView());
	/*
	cwc.backLeftBottom = inv * glm::vec4(-1, -1, -1, 1);
	cwc.backLeftTop    = inv * glm::vec4(-1, 1, -1, 1);
	cwc.backRightBottom= inv * glm::vec4(1, -1, -1, 1);
	cwc.backRightTop   = inv * glm::vec4(1, 1, -1, 1);
	cwc.frontLeftBottom= inv * glm::vec4(-1, -1, 1, 1);
	cwc.frontLeftTop   = inv * glm::vec4(-1, 1, 1, 1);
	cwc.frontRightBottom = inv * glm::vec4(1, -1, 1, 1);
	cwc.frontRightTop  = inv * glm::vec4(1,1,1,1);

	cwc.backLeftBottom /= cwc.backLeftBottom.w;
	cwc.backLeftTop    /= cwc.backLeftTop.w;
	cwc.backRightBottom/= cwc.backRightBottom.w;
	cwc.backRightTop   /= cwc.backRightTop.w;
	cwc.frontLeftBottom/= cwc.frontLeftBottom.w;
	cwc.frontLeftTop   /= cwc.frontLeftTop.w;
	cwc.frontRightBottom /= cwc.frontRightBottom.w;
	cwc.frontRightTop  /= cwc.frontRightTop.w;
	*/
	int index = 0;
	for (unsigned int x = 0; x < 2; ++x)
    {
        for (unsigned int y = 0; y < 2; ++y)
        {
            for (unsigned int z = 0; z < 2; ++z)
            {
                const glm::vec4 pt = inv * glm::vec4(2.0f * x - 1.0f, 2.0f * y - 1.0f, 2.0f * z - 1.0f, 1.0f);
                cwc.corners[index] = pt / pt.w;
				index += 1;
            }
        }
    }
	return cwc;
}

CameraWorldCorner Camera::GetFrustumCornersWorldSpace(float nearPlane, float farPlane)
{
	glm::mat4 proj = glm::perspective(glm::radians(m_Settings.zoom), (float)SCR_WIDTH / (float) SCR_HEIGHT, nearPlane, farPlane);
	return GetFrustumCornersWorldSpace(proj);
}



glm::vec3 Camera::GetFrustumWorldCenter()
{
	return GetFrustumCornersWorldSpace().Center();
}

float Camera::GetPitch()
{
	return m_Pitch;
}
float Camera::GetYaw()
{
	return m_Yaw;
}

void Camera::InverseY(float height)
{
	if (m_YInversed)
	{
		return;
	}

	m_Pos.y = height - (m_Pos.y - height);
	m_Pitch *= -1;
	glm::vec3 front;
	front.x = cos(glm::radians(m_Yaw))*cos(glm::radians(m_Pitch));
	front.y = sin(glm::radians(m_Pitch)); 
	front.z = sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
	m_CamFront = glm::normalize(front);
	m_YInversed = true;
}

void Camera::ReverseY(float height)
{
	if (!m_YInversed)
	{
		return;
	}
	m_Pos.y = height + (height - m_Pos.y);
	m_Pitch *= -1;
	glm::vec3 front;
	front.x = cos(glm::radians(m_Yaw))*cos(glm::radians(m_Pitch));
	front.y = sin(glm::radians(m_Pitch)); 
	front.z = sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
	m_CamFront = glm::normalize(front);
	
	m_YInversed = false;
}

void Camera::SetCamNav(bool nav)
{
	m_CamNav = nav;
}