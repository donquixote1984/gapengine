#pragma once
#include "../shaders/Shader.h"
#include <glm/glm.hpp>
#include <string>
#include "../render/DefaultMaterial.h"
#include "../geometry/Geometry.h"

class Primitive: public Geometry
{
protected:
    glm::vec3 m_DefaultColor;
public:
    Primitive(const std::string file);
    void InitMaterial() override;
};