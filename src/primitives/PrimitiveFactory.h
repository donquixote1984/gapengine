#pragma once

#include "Cube.h"
#include "Plane.h"
#include "Sphere.h"
#include "Torus.h"
#include "Cone.h"
#include "Monkey.h"
#include "Tube.h"
#include "Cylinder.h"
#include "PrimitiveType.h"
#include "../exceptions/InvalidTypeException.h"
#include "magic_enum.hpp"

class PrimitiveFactory
{
public:
    static Primitive* CreatePrimitive(PrimitiveType pType)
    {
        std::string typeStr;
        Primitive *p = NULL;
        if (pType == PrimitiveType::CUBE)
        {
            //Global::CreateGeometry(GeometryType::PRIMITIVE, )
            typeStr = "Cube.";
            p = new Cube();
            typeStr += std::to_string(Cube::counter);
        }
        else if (pType == PrimitiveType::PLANE)
        {
            typeStr = "Plane.";
            p = new Plane();
            typeStr += std::to_string(Plane::counter);
        }
        else if (pType == PrimitiveType::SPHERE)
        {
            typeStr = "Sphere.";
            p = new Sphere();
            typeStr += std::to_string(Sphere::counter);
        } 
        else if (pType == PrimitiveType::TORUS)
        {
            typeStr = "Torus.";
            p = new Torus();
            typeStr += std::to_string(Torus::counter);
        }
        else if (pType == PrimitiveType::CONE)
        {
            typeStr = "Cone.";
            p = new Cone();
            typeStr += std::to_string(Cone::counter);
        }
        else if (pType == PrimitiveType::CYLINDER)
        {
            typeStr = "Cylinder.";
            p = new Cylinder();
            typeStr += std::to_string(Cylinder::counter);
        }
        else if (pType == PrimitiveType::MONKEY)
        {
            typeStr = "Monkey.";
            p = new Monkey();
            typeStr += std::to_string(Monkey::counter);
        }
        else if (pType == PrimitiveType::TUBE)
        {
            typeStr = "Tube.";
            p = new Tube();
            typeStr += std::to_string(Tube::counter);
        }
        else {
            const std::string exp = static_cast<std::string>(magic_enum::enum_name(pType));
            throw InvalidTypeException(exp.c_str());
        }

        p->SetName(typeStr);
        return p;
    }
};