#pragma once
#include "MaterialValueNode.h"
#include "../render/PointLightMaterial.h"
#include "MaterialTemplate.h"
class PointLightMaterialValueNode: public MaterialValueNode
{
public:
    PointLightMaterialValueNode(MaterialTemplate *);
    void Calc() override;
};