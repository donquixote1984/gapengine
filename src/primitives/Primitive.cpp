#include "Primitive.h"
#include "../reader/ObjReader.h"
#include "../shaders/ShaderType.h"

Primitive::Primitive(const std::string file): Geometry(GeometryType::PRIMITIVE)
{
    ObjReader r(file);
    r >> this;
}

void Primitive::InitMaterial()
{
    m_Mat = new DefaultMaterial();
}