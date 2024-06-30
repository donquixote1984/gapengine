#pragma once

#include "Cube.h"
#include "Plane.h"
#include "Sphere.h"
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
        } else { 
            const std::string exp = static_cast<std::string>(magic_enum::enum_name(pType));
            throw InvalidTypeException(exp.c_str());
        }

        p->SetName(typeStr);
        return p;
    }
};