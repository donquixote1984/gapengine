#include "PointLightMaterialValueNode.h"
#include "../render/LightMaterial.h"

PointLightMaterialValueNode::PointLightMaterialValueNode(MaterialTemplate *mt): MaterialValueNode(mt)
{
}
void PointLightMaterialValueNode::Calc()
{

    LightMaterial *material;
    if ((m_Outputs[0].GetPtr()) != nullptr)
    {
        material = (PointLightMaterial *)m_Outputs[0].GetPtr();
    } else {
        material = new PointLightMaterial();
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

        if (channelIndex == MaterialValueNodeEnum::LINEAR)
        {
            MakeMaterialAttrib(material->linear, GetInputValueByKey((unsigned int)MaterialValueNodeEnum::LINEAR));
        }

        if (channelIndex == MaterialValueNodeEnum::CONSTANT)
        {
            MakeMaterialAttrib(material->constant, GetInputValueByKey((unsigned int)MaterialValueNodeEnum::CONSTANT));
        }

        if (channelIndex == MaterialValueNodeEnum::QUADRATIC)
        {
            MakeMaterialAttrib(material->quadratic, GetInputValueByKey((unsigned int)MaterialValueNodeEnum::QUADRATIC));
        }
    }
}