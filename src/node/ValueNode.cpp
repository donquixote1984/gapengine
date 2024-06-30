#include "ValueNode.h"

ValueNodeValue::ValueNodeValue()
{
    m_Type = ValueNodeType::FLOAT;
    m_Value = 0;
}

ValueNodeValue::ValueNodeValue(const ValueNodeValue &v)
{
    m_Type = v.m_Type;
    m_Value = v.m_Value;
}
ValueNodeValue::ValueNodeValue(float x)
{
    m_Type = ValueNodeType::FLOAT;
    m_Value = std::make_any<float>(x);
}

ValueNodeValue::ValueNodeValue(glm::vec3 x)
{
    m_Type = ValueNodeType::VECTOR;
    m_Value = std::make_any<glm::vec3>(x);
}

ValueNodeValue::ValueNodeValue(void *ptr)
{
    if (ptr == nullptr)
    {
        m_Type = ValueNodeType::NONE;
        m_Value = (void *)nullptr;
    } else {
        m_Type = ValueNodeType::POINTER;
        m_Value = ptr;
    }
}

ValueNodeValue::ValueNodeValue(std::string str)
{
    m_Type = ValueNodeType::STRING;
    m_Value = std::make_any<std::string>(str);
}

ValueNodeValue::ValueNodeValue(ValueNodeType type, std::any value)
{
    m_Type = type;
    m_Value = value;
}

ValueNodeType ValueNodeValue::GetType()
{
    return m_Type;
}

glm::vec3 ValueNodeValue::GetVector() 
{
    return std::any_cast<glm::vec3>(m_Value);
}

std::pair<glm::vec3, void *> ValueNodeValue::GetPair()
{
    return std::any_cast<std::pair<glm::vec3, Texture *>>(m_Value);
}
float ValueNodeValue::GetFloat()
{
    return std::any_cast<float>(m_Value);
}
void * ValueNodeValue::GetPtr()
{
    return std::any_cast<void *>(m_Value);
}

std::string ValueNodeValue::GetStr()
{
    return std::any_cast<std::string>(m_Value);
}

ValueNode::ValueNode()
{
    this->InitInputs();
    this->InitOutputs();
    //Calc();
}

ValueNode::~ValueNode()
{
    for (int i = 0; i < m_Inputs.size(); i++)
    {
        delete m_Inputs[i].valueNode;
    }
}

// for changing inputs node after init
void ValueNode::UpdateInputValue(unsigned int key, ValueNode* newValueNode)
{
    for (int i = 0; i < m_Inputs.size(); i++)
    {
        if (m_Inputs[i].key == key)
        {
            ValueNode * valueNode = m_Inputs[i].valueNode;
            if (valueNode != NULL && valueNode !=nullptr)
            {
                delete valueNode;
                valueNode = NULL;
            }
            m_Inputs[i].valueNode = newValueNode;
            break;
        }
    }
    
    Calc();
}

ValueNodeType ValueNode::GetType() {
    return ValueNodeType::RAW;
}

void ValueNode::Calc()
{
}

ValueNodeValue ValueNode::GetInputValueByKey(unsigned int key)
{
    for (int i = 0; i < m_Inputs.size(); i++)
    {
        if (m_Inputs[i].key == key)
        {
            return m_Inputs[i].valueNode->GetFirstOutputValue();
        }
    }

    return ValueNodeValue();
}

ValueNode * ValueNode::GetInputValueNodeByKey(unsigned int key)
{
    for (int i = 0; i < m_Inputs.size(); i++)
    {
        if (m_Inputs[i].key == key)
        {
            return m_Inputs[i].valueNode;
        }
    }
    return nullptr;
}
ValueNodeValue ValueNode::GetFirstOutputValue()
{
    return m_Outputs[0];
}