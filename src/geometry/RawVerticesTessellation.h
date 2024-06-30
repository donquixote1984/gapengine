#pragma once
#include "RawVertices.h"
class RawVerticesTessellation: public RawVertices
{
protected:
    unsigned int m_Unit = 4;
public:
    RawVerticesTessellation(float * vertices, int count, std::initializer_list<int> layouts);
    virtual void Draw();
    virtual void Draw(DisplayMode mode);
};