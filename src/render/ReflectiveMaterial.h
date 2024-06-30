#pragma once
#include "Material.h"
#include "../shaders/GeometryShader.h"
#include "MaterialAttrib.h"
#include "../global/GlobalData.h"

class ReflectiveMaterial: public Material
{
public:
    ReflectiveMaterial();
    MaterialAttrib reflection;
    void InitAttribs() override;
    void FlushToShader() override;
};