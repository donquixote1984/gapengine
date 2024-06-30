#include "ValueNode.h"
#include "ColorValueNode.h"
#include "../textures/Texture.h"


ColorValueNode::ColorValueNode(): ValueNode()
{
    InitInputs();
    InitOutputs();
    
}

ColorValueNode::ColorValueNode(glm::vec3 & input): ColorValueNode()
{
    AttachColor(input.x, input.y, input.z);
}

ColorValueNode::ColorValueNode(float x, float y, float z): ColorValueNode()
{
    AttachColor(x, y, z);
}

ColorValueNode::ColorValueNode(float x, float y, float z, const std::string &filePath): ColorValueNode()
{
    AttachColor(x, y, z);
    AttachTexture(filePath);
}

void ColorValueNode::AttachColor(float x, float y , float z)
{
    BasicValueNode * bvn = static_cast<BasicValueNode *>(m_Inputs[0].valueNode);
    bvn->SetValue(glm::vec3(x, y, z));
    Calc();
}

void ColorValueNode::AttachTexture(const std::string &filePath)
{
    //AttachTexture(filePath);
    BasicValueNode * bvn2 = static_cast<BasicValueNode *>(m_Inputs[1].valueNode);
    bvn2->SetValue(filePath);
    Calc();
}

ColorValueNode::ColorValueNode(const std::string &filePath): ColorValueNode()
{
    BasicValueNode * bvn2 = static_cast<BasicValueNode *>(m_Inputs[1].valueNode);
    bvn2->SetValue(filePath);
    Calc();
}


void ColorValueNode::InitInputs()
{
    m_InputNum = 2;
    m_Inputs.push_back({ValueSlotType::VECTOR, 0, "basecolor", new VectorValueNode(1.0f, 1.0f, 1.0f)});
    m_Inputs.push_back({ValueSlotType::TEXTURE, 1, "texture", new TextureNode("")});
}

void ColorValueNode::InitOutputs()
{
    m_OutputNum = 1;
    std::pair<glm::vec3, Texture *> pair {glm::vec3(1.0f), nullptr};

    //std::any t = std::make_any<std::tuple>(tuple);
    ValueNodeValue vnv = ValueNodeValue(ValueNodeType::PAIR, pair);
    m_Outputs.push_back(vnv);
}

void ColorValueNode::Calc()
{
    ValueNodeValue vec3Value = m_Inputs[0].valueNode->GetFirstOutputValue();
    //Texture * t = m_
    ValueNodeValue textureValue = m_Inputs[1].valueNode->GetFirstOutputValue();

    const glm::vec3 vec = vec3Value.GetVector();
    Texture * t =  (Texture *)textureValue.GetPtr();

    //std::pair pair= std::make_pair<glm::vec3, Texture*>(vec, t);
    std::pair<glm::vec3, Texture *> pair {vec, t};
    ValueNodeValue vnv = {ValueNodeType::PAIR, pair};

    m_Outputs[0] = vnv;
}

ValueNodeType ColorValueNode::GetType()
{
    return ValueNodeType::PAIR;
}