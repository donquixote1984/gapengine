#pragma once

#include <iostream>
#include <glm/glm.hpp>
#include "../textures/Texture.h"

enum class MaterialAttribType
{
    FLOAT, 
    COLOR,
    STRING
};

class Texture;
class MaterialAttrib
{
public:
    MaterialAttribType type;
    const char * uniform;
    Texture *texture = nullptr;
    glm::vec3 color = glm::vec3(1.0f);
    float constant;
    bool enable = true;
    bool reverseY = false;

    MaterialAttrib();
    ~MaterialAttrib();
    MaterialAttrib(float constant);
    MaterialAttrib(glm::vec3 vec);
    MaterialAttrib(Texture* t);
    
};