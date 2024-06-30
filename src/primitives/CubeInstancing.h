#pragma once
#include "Cube.h"
#include "PrimitiveInstancing.h"

class CubeInstancing: public PrimitiveInstancing
{
public:
    CubeInstancing();
};


//primitiveinstancing -> objreader -> globaldata-> primitiveinstancingfactory -> primitiveinstancing