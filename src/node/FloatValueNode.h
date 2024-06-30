#pragma once
#include "ValueNode.h"
#include "BasicValueNode.h"
#include <any>

enum class FloatValueKey {
    FLOAT_VALUE
};
class FloatValueNode: public BasicValueNode
{
public:
    FloatValueNode();
    FloatValueNode(float init);
    void InitInputs() override;
    void InitOutputs() override;
    ValueNodeType GetType() override;
    void SetValue(ValueNodeValue v) override;
    void Calc() override;
};