#pragma once
#include "ValueNode.h"
#include "../textures/Texture.h"
#include "StringValueNode.h"
#include "BasicValueNode.h"

enum class TextureValueNodeEnum
{
    FILE
};

class TextureNode: public BasicValueNode
{
public:
    TextureNode();
    TextureNode(std::string);
    void InitInputs() override;
    void InitOutputs() override;
    void Calc() override;
    ValueNodeType GetType() override;
    void SetValue(ValueNodeValue v) override;
};