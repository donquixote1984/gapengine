#include "CustomMaterialValueNode.h"

CustomMaterialValueNode::CustomMaterialValueNode(MaterialTemplate * mt): MaterialValueNode(mt)
{
}

void CustomMaterialValueNode::Calc()
{
    ValueNodeValue vnv = GetInputValueByKey((unsigned int)MaterialValueNodeEnum::CUSTOM);
    std::string shaderpath;
    try {
        shaderpath = vnv.GetStr();
    } catch (std::exception e)
    {
        throw MissingConfigurationException("custom material does not have 'shader' attrib ");
    }
    GetOrCreateMaterial<CustomMaterial>(shaderpath);
}
