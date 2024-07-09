#include "Monkey.h"
#include <iterator>

Monkey::Monkey() :Primitive("res/primitives/Monkey.obj")
{
	// InitGeometry("res/primitives/cube.geo", GeoType::POSITION_AND_NORMAL);
	counter += 1;
}
int Monkey::counter = 0;