#pragma once
#include "ValueNode.h"
#include "BasicValueNode.h"
enum class StringValueEnum 
{
    STRING_VALUE
};
class StringValueNode: public BasicValueNode
{
public:
    StringValueNode();
    StringValueNode(std::string init);
    void InitInputs() override;
    void InitOutputs() override;
    ValueNodeType GetType() override;
    void SetValue(ValueNodeValue v) override;
};