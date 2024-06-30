#pragma once
#include "ValueNode.h"
#include "FloatValueNode.h"
#include "BasicValueNode.h"
#include <glm/glm.hpp>
#include <any>

enum class VectorValueNodeEnum { X, Y, Z };

class VectorValueNode: public BasicValueNode
{
public:
    VectorValueNode();
    VectorValueNode(float x, float y, float z);
    void InitInputs() override;
    void InitOutputs() override;
    ValueNodeType GetType() override;
    void SetValue(ValueNodeValue) override;
    void Calc() override;
};