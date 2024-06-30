#include "ReflectiveMaterialValueNode.h"

ReflectiveMaterialValueNode::ReflectiveMaterialValueNode(MaterialTemplate * mt):MaterialValueNode(mt)
{
}

void ReflectiveMaterialValueNode::Calc()
{
    ReflectiveMaterial * material = GetOrCreateMaterial<ReflectiveMaterial>();
   
    const std::vector<MaterialTemplateItem> &channels = m_Template->GetTemplate();
    for (int i = 0; i < channels.size(); i ++ )
    {
        MaterialValueNodeEnum channelIndex = channels[i].channelIndex;
        if (channelIndex == MaterialValueNodeEnum::AMBIENT)
        {
            MakeMaterialAttrib(material->reflection, GetInputValueByKey((unsigned int)MaterialValueNodeEnum::REFLECTION));
        }
    }
}