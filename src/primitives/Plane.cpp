#include "Plane.h"

Plane::Plane():Primitive("res/primitives/plane.obj")
{

    Plane::counter += 1;
}
int Plane::counter = 0;