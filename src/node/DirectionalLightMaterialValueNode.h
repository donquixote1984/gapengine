#pragma once
#include "MaterialValueNode.h"
#include "../render/DirectionalLightMaterial.h"
#include "MaterialTemplate.h"

class DirectionalLightMaterialValueNode: public MaterialValueNode
{
public:
    DirectionalLightMaterialValueNode(MaterialTemplate *);
    void Calc() override;
};