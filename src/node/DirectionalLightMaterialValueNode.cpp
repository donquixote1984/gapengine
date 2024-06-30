#include "DirectionalLightMaterialValueNode.h"
#include "../render/LightMaterial.h"

DirectionalLightMaterialValueNode::DirectionalLightMaterialValueNode(MaterialTemplate * mt): MaterialValueNode(mt)
{}
void DirectionalLightMaterialValueNode::Calc()
{

    LightMaterial *material;
    if ((m_Outputs[0].GetPtr()) != nullptr)
    {
        material = (DirectionalLightMaterial *)m_Outputs[0].GetPtr();
    } else {
        material = new DirectionalLightMaterial();
        ValueNodeValue vnv = (void *) material;
        m_Outputs[0] = vnv;
    }

    const std::vector<MaterialTemplateItem> &channels = m_Template->GetTemplate();
    for (int i = 0; i < channels.size(); i ++ )
    {
        MaterialValueNodeEnum channelIndex = channels[i].channelIndex;
        if (channelIndex == MaterialValueNodeEnum::AMBIENT)
        {
            MakeMaterialAttrib(material->ambient, GetInputValueByKey((unsigned int)MaterialValueNodeEnum::AMBIENT));
        }
        if (channelIndex == MaterialValueNodeEnum::DIFFUSE)
        {
            MakeMaterialAttrib(material->diffuse, GetInputValueByKey((unsigned int)MaterialValueNodeEnum::DIFFUSE));
        }
        if (channelIndex == MaterialValueNodeEnum::SPECULAR)
        {
            MakeMaterialAttrib(material->specular, GetInputValueByKey((unsigned int)MaterialValueNodeEnum::SPECULAR));
        }
    }
}