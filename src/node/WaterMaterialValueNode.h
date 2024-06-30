#pragma once
#include "MaterialValueNode.h"
#include "../render/WaterMaterial.h"
#include "MaterialTemplate.h"

class WaterMaterialValueNode: public MaterialValueNode
{
public:
    WaterMaterialValueNode(MaterialTemplate*);
    void Calc() override;
};