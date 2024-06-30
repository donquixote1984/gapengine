#include "VectorValueNode.h"
#include "ValueSlot.h"
#include "FloatValueNode.h"
#include <glm/glm.hpp>

VectorValueNode::VectorValueNode():BasicValueNode()
{
    InitInputs();
    InitOutputs();
}

VectorValueNode::VectorValueNode(float x, float y, float z): VectorValueNode()
{
    //m_Inputs[0].value = FloatValueNode(x);
    //m_Inputs[1].value = FloatValueNode(y);
    //m_Inputs[2].value = FloatValueNode(z);
    SetValue(glm::vec3(x, y, z));
    Calc();
}


void VectorValueNode::SetValue(ValueNodeValue vnv)
{
    if (vnv.GetType() == ValueNodeType::VECTOR)
    {
        glm::vec3 vec = vnv.GetVector();
        m_Intrinsic[(unsigned int)VectorValueNodeEnum::X] = vec.x;
        m_Intrinsic[(unsigned int)VectorValueNodeEnum::Y] = vec.y;
        m_Intrinsic[(unsigned int)VectorValueNodeEnum::Z] = vec.z;
        Calc();
    }
}
void VectorValueNode::InitInputs()
{
    m_InputNum = 0;
    //m_Inputs.push_back({ValueSlotType::INPUT, (unsigned int)VectorValueNodeEnum::X, "x", FloatValueNode()});
    //m_Inputs.push_back({ValueSlotType::INPUT, (unsigned int)VectorValueNodeEnum::Y, "y", FloatValueNode()});
    //m_Inputs.push_back({ValueSlotType::INPUT, (unsigned int)VectorValueNodeEnum::Z, "z", FloatValueNode()});
}

void VectorValueNode::InitOutputs()
{
    m_OutputNum = 1;
    ValueNodeValue vnv = glm::vec3(0);
    m_Outputs.push_back(vnv);
}

void VectorValueNode::Calc()
{
    //FloatValueNode x = std::any_cast<FloatValueNode>(GetInputValueByKey((unsigned int)VectorValueNodeEnum::X));
    //FloatValueNode y = std::any_cast<FloatValueNode>(GetInputValueByKey((unsigned int)VectorValueNodeEnum::X));
    //FloatValueNode z = std::any_cast<FloatValueNode>(GetInputValueByKey((unsigned int)VectorValueNodeEnum::X));
    
    float x = m_Intrinsic[0].GetFloat();
    float y = m_Intrinsic[1].GetFloat();
    float z = m_Intrinsic[2].GetFloat();
    m_Outputs[0] = glm::vec3(x, y, z);
}

ValueNodeType VectorValueNode::GetType()
{
    return ValueNodeType::VECTOR;
}