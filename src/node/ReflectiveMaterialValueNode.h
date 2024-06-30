#pragma once

#include "../render/ReflectiveMaterial.h"
#include "MaterialTemplate.h"
#include "MaterialValueNode.h"

class ReflectiveMaterialValueNode: public MaterialValueNode
{
public:
    ReflectiveMaterialValueNode(MaterialTemplate*);
    void Calc() override;
};