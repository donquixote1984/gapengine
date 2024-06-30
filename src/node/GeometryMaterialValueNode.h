#pragma once
#include "MaterialValueNode.h"
#include "../render/DefaultMaterial.h"
#include "../render/PBRMaterial.h"
#include "../render/TerrainMaterial.h"
#include "MaterialTemplate.h"
class GeometryMaterialValueNode: public MaterialValueNode
{
public:
    GeometryMaterialValueNode(MaterialTemplate*);
    void Calc() override;
};