#include "Cylinder.h"
#include <iterator>

Cylinder::Cylinder() :Primitive("res/primitives/cylinder.obj")
{
	// InitGeometry("res/primitives/cube.geo", GeoType::POSITION_AND_NORMAL);
	counter += 1;
}
int Cylinder::counter = 0;