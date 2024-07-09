#include "Tube.h"
#include <iterator>

Tube::Tube() :Primitive("res/primitives/tube.obj")
{
	// InitGeometry("res/primitives/cube.geo", GeoType::POSITION_AND_NORMAL);
	counter += 1;
}
int Tube::counter = 0;