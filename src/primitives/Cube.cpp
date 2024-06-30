#include "Cube.h"
#include <iterator>

Cube::Cube():Primitive("res/primitives/cube.obj")
{
   // InitGeometry("res/primitives/cube.geo", GeoType::POSITION_AND_NORMAL);
   counter+= 1;
}
int Cube::counter= 0;