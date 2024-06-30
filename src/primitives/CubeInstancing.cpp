#include "CubeInstancing.h"

CubeInstancing::CubeInstancing(): PrimitiveInstancing("res/primitives/cube.obj")
{
    Cube::counter += 1;
}