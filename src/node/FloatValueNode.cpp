#include "FloatValueNode.h"
#include "ValueSlot.h"

FloatValueNode::FloatValueNode():BasicValueNode()
{
    m_Intrinsic[(unsigned int)FloatValueKey::FLOAT_VALUE] = 0.0f;
    InitInputs();
    InitOutputs();
}

FloatValueNode::FloatValueNode(float init): BasicValueNode()
{
    //m_Inputs[0].value = init;
    m_Intrinsic[(unsigned int)FloatValueKey::FLOAT_VALUE] = init;
    InitInputs();
    InitOutputs();
    //Calc();
}

void FloatValueNode::InitInputs()
{
    m_InputNum = 0;
    //ValueSlotInput<float> vs = {ValueSlotType::VALUE, 0, "value", 0};
    //m_Inputs.push_back(vs);
}

void FloatValueNode::InitOutputs()
{
    m_OutputNum = 1;
    //ValueSlotOutput<float> vs = {0, "value", 0};
    ValueNodeValue vnv = m_Intrinsic[(unsigned int)FloatValueKey::FLOAT_VALUE];
    m_Outputs.push_back(vnv);
}
ValueNodeType FloatValueNode::GetType()
{
    return ValueNodeType::FLOAT;
}

void FloatValueNode::SetValue(ValueNodeValue v)
{
    m_Intrinsic[0]= v;
    Calc();
}

void FloatValueNode::Calc()
{
    ValueNodeValue vnv = m_Intrinsic[(unsigned int)FloatValueKey::FLOAT_VALUE];
    m_Outputs[0]= vnv;
}