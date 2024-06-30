#pragma once

#include "ValueNode.h"
class BasicValueNode: public ValueNode
{
public:
    virtual void SetValue(ValueNodeValue);
    virtual void Calc() override;
    ValueNodeType GetType() override;
};