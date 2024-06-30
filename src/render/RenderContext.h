#pragma once
#include <glm/glm.hpp>
#include "../Camera.h"
struct RenderContext
{
    glm::mat4 projection;
    glm::mat4 view;
    glm::vec3 camPos;
    Camera * camera;
    unsigned int frame;
    float deltaTime;
};