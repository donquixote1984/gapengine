#pragma once
#include "VolumeMaterial.h"
class SimpleCloudMaterial: public VolumeMaterial 
{
public:
    MaterialAttrib density;

    SimpleCloudMaterial();
    ~SimpleCloudMaterial();

    void FlushToShader() override;
    void InitAttribs() override;
};