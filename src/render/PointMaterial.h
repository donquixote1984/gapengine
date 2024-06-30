#pragma once
#include "Material.h"
#include "MaterialAttrib.h"
#include "../shaders/ShaderType.h"
#include "../shaders/GeometryShader.h"

class PointMaterial: public Material
{
private:
    MaterialAttrib color;
    MaterialAttrib size;

public:
    PointMaterial();
    void FlushToShader() override;
    //virtual void MVPToShader(glm::mat4 model, glm::mat4 view, glm::mat4 projection, glm::vec3 camPos) const = 0;
    void InitAttribs() override;
};