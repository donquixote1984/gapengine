#include "MaterialValueNode.h"
#include "../render/MaterialAttrib.h"


MaterialValueNode::MaterialValueNode(MaterialTemplate * materialTemplate):ValueNode()
{
    m_Template = materialTemplate;
    InitInputs();
    InitOutputs();
}

void MaterialValueNode::MakeMaterialAttrib(MaterialAttrib & mAttrib, ValueNodeValue value)
{
    if (value.GetType() == ValueNodeType::PAIR)
    {
        std::pair pair = value.GetPair();
        //mAttrib.type = 
        mAttrib.color = pair.first;
        mAttrib.texture = (Texture *)pair.second;
        mAttrib.type = MaterialAttribType::COLOR;

    } else if(value.GetType() == ValueNodeType::FLOAT)
    {
        mAttrib.constant = value.GetFloat();

    }
}

void MaterialValueNode::InitInputs()
{
    const std::vector<MaterialTemplateItem> &channels = m_Template->GetTemplate();
    m_InputNum = channels.size();

    for (int i = 0 ; i < channels.size() ; i++)
    {
        MaterialAttribType type = channels[i].channelType;
        std::string channelName = channels[i].channelName;
        MaterialValueNodeEnum materialChannel = channels[i].channelIndex;
        
        if (type == MaterialAttribType::COLOR) {
            glm::vec3 defaultValue = std::any_cast<glm::vec3>(channels[i].defaultValue);
            m_Inputs.push_back({ValueSlotType::COLOR, (unsigned int)materialChannel, channelName, new ColorValueNode(defaultValue)});
        } else if (type == MaterialAttribType::STRING) 
        {
            m_Inputs.push_back({ValueSlotType::STRING, (unsigned int)materialChannel, channelName, new StringValueNode("")});
        } else {
            float defaultValue = std::any_cast<float>(channels[i].defaultValue);
            m_Inputs.push_back({ValueSlotType::FLOAT, (unsigned int)materialChannel, channelName, new FloatValueNode(defaultValue)});
        }
    }

    /**
     *     float shininess=32.0f;
    // point light
    float linear=0.09;
    float constant=1.0f;
    float quadratic=0.032f;
    */
}
void MaterialValueNode::InitOutputs()
{
    m_OutputNum = 1;
    ValueNodeValue vnv = ValueNodeValue(ValueNodeType::POINTER, (void *)nullptr);
    m_Outputs.push_back(vnv);
}

ValueNodeType MaterialValueNode::GetType()
{
    return ValueNodeType::MATERIAL;
}

void MaterialValueNode::Calc()
{
}