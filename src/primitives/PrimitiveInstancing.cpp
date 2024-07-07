#include "PrimitiveInstancing.h"
#include "../reader/ObjReader.h"


PrimitiveInstancing::PrimitiveInstancing(const std::string &objPath): GeometryInstancing(GeometryType::PRIMITIVE)
{
    ObjReader r(objPath);
    r >> this;
}

void PrimitiveInstancing::InitMaterial()
{
    //m_Mat = new DefaultMaterial();
    m_Mats.push_back(new DefaultMaterial());
}