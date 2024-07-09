
#pragma once
#include "CubeInstancing.h"
#include "SphereInstancing.h"
#include <string>
#include "../geometry/Geometry.h"
#include "PrimitiveType.h"

class PrimitiveInstancingFactory
{
public:
    static Geometry * CreatePrimitiveInstancing(PrimitiveType pType)
    {
        std::string typeStr;
        Geometry *p = NULL;
        
        if (pType == PrimitiveType::CUBE)
        {
            //Global::CreateGeometry(GeometryType::PRIMITIVE, )
            typeStr = "CubeInstancing.";
            p = new CubeInstancing();
            typeStr += std::to_string(Cube::counter);
        }
        else if (pType == PrimitiveType::PLANE)
        {
        }
        else if (pType == PrimitiveType::SPHERE)
        {
            typeStr = "SphereInstancing.";
            p = new SphereInstancing();
            typeStr += std::to_string(Sphere::counter);

        } else { 
            const std::string exp = static_cast<std::string>(magic_enum::enum_name(pType));
            throw InvalidTypeException(exp.c_str());
        }

        p->SetName(typeStr);
        return p;
    }
};