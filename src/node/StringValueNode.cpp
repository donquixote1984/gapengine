#include "StringValueNode.h"
#include "ValueSlot.h"

StringValueNode::StringValueNode():BasicValueNode()
{
    InitInputs();
    InitOutputs();
}

StringValueNode::StringValueNode(std::string init) 
{
    //m_Inputs[0].value = init;
    m_Intrinsic[(unsigned int)StringValueEnum::STRING_VALUE] = init;
    InitInputs();
    InitOutputs();
}

void StringValueNode::InitInputs()
{
    m_InputNum = 0;
    //ValueSlotInput<std::string> vs = {ValueSlotType::VALUE, 0, "value", ""};
    //m_Inputs.push_back(vs);
}

void StringValueNode::InitOutputs()
{
    m_OutputNum = 1;
    ValueNodeValue vs =  m_Intrinsic[(unsigned int)StringValueEnum::STRING_VALUE];
    m_Outputs.push_back(vs);
}

ValueNodeType StringValueNode::GetType()
{
    return ValueNodeType::FLOAT;
}

void StringValueNode::SetValue(ValueNodeValue v)
{
    m_Intrinsic[0] = v;
}