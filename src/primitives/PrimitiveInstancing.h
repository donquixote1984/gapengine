#pragma once
#include <string>
#include "../render/DefaultMaterial.h"
#include "../geometry/GeometryInstancing.h"


class PrimitiveInstancing : public GeometryInstancing
{
public:
    PrimitiveInstancing(const std::string &objPath);
    void InitMaterial() override;
};