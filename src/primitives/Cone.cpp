#include "Cone.h"
#include <iterator>

Cone::Cone() :Primitive("res/primitives/cone.obj")
{
	// InitGeometry("res/primitives/cube.geo", GeoType::POSITION_AND_NORMAL);
	counter += 1;
}
int Cone::counter = 0;