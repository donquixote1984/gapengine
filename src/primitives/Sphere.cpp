#include "Sphere.h"

Sphere::Sphere():Primitive("res/primitives/sphere.obj")
{
    Sphere::counter +=1;
}
int Sphere::counter = 0;