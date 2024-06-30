#include "MaterialAttrib.h"

MaterialAttrib::MaterialAttrib()
{
}
MaterialAttrib::~MaterialAttrib()
{
    if (texture != nullptr)
    {
        delete texture;
        texture = nullptr;
    }
}
MaterialAttrib::MaterialAttrib(float constant):constant(constant), type(MaterialAttribType::FLOAT)
{
}
MaterialAttrib::MaterialAttrib(glm::vec3 vec):color(vec), texture(nullptr), type(MaterialAttribType::COLOR)
{
}
MaterialAttrib::MaterialAttrib(Texture* t):color(glm::vec3(1.0f)), type(MaterialAttribType::COLOR)
{
    if (t != nullptr) {
        delete texture;
    }
    texture = t;
}