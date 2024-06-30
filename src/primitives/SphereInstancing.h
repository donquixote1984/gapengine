#pragma once
#include "Sphere.h"
#include "PrimitiveInstancing.h"

class SphereInstancing: public PrimitiveInstancing
{
public:
    SphereInstancing();
};


//primitiveinstancing -> objreader -> globaldata-> primitiveinstancingfactory -> primitiveinstancing