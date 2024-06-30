#include "SphereInstancing.h"

SphereInstancing::SphereInstancing(): PrimitiveInstancing("res/primitives/sphere.obj")
{
    Sphere::counter += 1;
}