#include "ValueNode.h"
#include "TextureNode.h"
#include "../textures/Texture.h"


TextureNode::TextureNode(): BasicValueNode()
{
    InitInputs();
    InitOutputs();

}

TextureNode::TextureNode(std::string filePath): TextureNode()
{
    m_Intrinsic[(unsigned int)TextureValueNodeEnum::FILE] = filePath;
    Calc();
}

void TextureNode::InitInputs()
{
    m_InputNum = 0;
   // m_InputNum = 1;
   // m_Inputs.push_back({ValueSlotType::VALUE, 0, "file", StringValueNode("")});
}

void TextureNode::InitOutputs()
{
    m_OutputNum = 1;
    ValueNodeValue vnv = (void *)nullptr;
    m_Outputs.push_back(vnv);
}

void TextureNode::Calc()
{
    if (m_Outputs[0].GetType() != ValueNodeType::NONE) 
    {
        return;
    }

    std::string filePath = m_Intrinsic[0].GetStr();
    
    if (filePath.empty())
    {
        m_Outputs[0] = (void *)nullptr;
        return;
    }

    Texture *texture = new Texture(filePath);
    if (m_Outputs[0].GetPtr()!= nullptr)
    {
        delete (Texture *)m_Outputs[0].GetPtr();
    }
    m_Outputs[0] = (void *)texture;
}

ValueNodeType TextureNode::GetType()
{
    return ValueNodeType::TEXTURE;
}

void TextureNode::SetValue(ValueNodeValue v)
{
    m_Intrinsic[0] = v;
    Calc();
}