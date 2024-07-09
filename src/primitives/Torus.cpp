#include "Torus.h"
#include <iterator>

Torus::Torus() :Primitive("res/primitives/torus.obj")
{
	// InitGeometry("res/primitives/cube.geo", GeoType::POSITION_AND_NORMAL);
	counter += 1;
}
int Torus::counter = 0;