#pragma once
#include "MaterialValueNode.h"
#include "../render/CustomMaterial.h"
#include "MaterialTemplate.h"
#include "../exceptions/MissingConfigurationException.h"

class CustomMaterialValueNode : public MaterialValueNode
{
public:
    CustomMaterialValueNode(MaterialTemplate *);
    void Calc() override;
};