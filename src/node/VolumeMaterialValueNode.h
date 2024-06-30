#pragma once
#include "MaterialValueNode.h"
#include "../render/VolumeMaterial.h"
#include "MaterialTemplate.h"
#include "../render/CloudMaterial.h"
#include "../render/TerrainSDFMaterial.h"
class VolumeMaterialValueNode: public MaterialValueNode
{
public:
    VolumeMaterialValueNode(MaterialTemplate*);
    void Calc() override;
};