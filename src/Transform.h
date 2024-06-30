#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <math.h>
#include "util/Util.h"

struct Transform
{
    glm::vec3 position = glm::vec3(0.0);
    glm::vec3 rotate = glm::vec3(0.0, 1.0, 0.0);
    glm::vec3 scale = glm::vec3(1.0);
    float angle = 0;

    Transform();
    Transform(glm::vec3 pos);

    glm::mat4 ToMatrix4();
    glm::mat4 ToMatrix4(float scaleVariant);
    glm::mat4 ToMatrix4(float scaleVariant, glm::vec3 angleAxis);
};