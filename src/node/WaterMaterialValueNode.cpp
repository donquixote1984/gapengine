#include "WaterMaterialValueNode.h"
WaterMaterialValueNode::WaterMaterialValueNode(MaterialTemplate *mt): MaterialValueNode(mt)
{}
void WaterMaterialValueNode::Calc()
{
    WaterMaterial * material = GetOrCreateMaterial<WaterMaterial>();
    
    const std::vector<MaterialTemplateItem> &channels = m_Template->GetTemplate();
    for (int i = 0; i < channels.size(); i ++ )
    {
        MaterialValueNodeEnum channelIndex = channels[i].channelIndex;
        if (channelIndex == MaterialValueNodeEnum::TINT)
        {
            MakeMaterialAttrib(material->tint, GetInputValueByKey((unsigned int)MaterialValueNodeEnum::TINT));
        }
        if (channelIndex == MaterialValueNodeEnum::TRANSPARENCY)
        {
            MakeMaterialAttrib(material->transparency, GetInputValueByKey((unsigned int)MaterialValueNodeEnum::TRANSPARENCY));
        }
        if (channelIndex == MaterialValueNodeEnum::FLOW)
        {
            MakeMaterialAttrib(material->flow, GetInputValueByKey((unsigned int)MaterialValueNodeEnum::FLOW));
        }
        if (channelIndex == MaterialValueNodeEnum::WAVE_STRENGTH)
        {
            MakeMaterialAttrib(material->waveStrength, GetInputValueByKey((unsigned int)MaterialValueNodeEnum::WAVE_STRENGTH));
        }

        if (channelIndex == MaterialValueNodeEnum::WAVE_SPEED)
        {
            MakeMaterialAttrib(material->waveSpeed, GetInputValueByKey((unsigned int)MaterialValueNodeEnum::WAVE_SPEED));
        }

        if (channelIndex == MaterialValueNodeEnum::NORMAL)
        {
            MakeMaterialAttrib(material->normal, GetInputValueByKey((unsigned int)MaterialValueNodeEnum::NORMAL));
        }
    }
}