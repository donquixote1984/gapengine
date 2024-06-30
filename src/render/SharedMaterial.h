#pragma once
#include "Material.h"
#include "PointMaterial.h"


class SharedMaterial
{
public:
    static Material * GetSharedPointMaterial()
    {
        static PointMaterial pm;
        return &pm;
    }
};