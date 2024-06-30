#include "BasicValueNode.h"

void BasicValueNode::SetValue(ValueNodeValue value)
{
    m_Intrinsic[0] = value;
    Calc();
}

void BasicValueNode::Calc()
{
    m_Outputs[0] = m_Intrinsic[0];
}

ValueNodeType BasicValueNode::GetType() {
    return ValueNodeType::NONE;
}