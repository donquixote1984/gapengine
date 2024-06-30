#include "VolumeMaterialValueNode.h"
VolumeMaterialValueNode::VolumeMaterialValueNode(MaterialTemplate *mt): MaterialValueNode(mt)
{}
void VolumeMaterialValueNode::Calc()
{
    VolumeMaterial * material = nullptr;
    if (m_Template->HasVariation(MaterialTemplateVariation::CLOUD))
    {
        material = GetOrCreateMaterial<CloudMaterial>();
    } 
    else if (m_Template->HasVariation(MaterialTemplateVariation::TERRAIN_SDF))
    {
        material = GetOrCreateMaterial<TerrainSDFMaterial>();
    }
    else {
        material = GetOrCreateMaterial<VolumeMaterial>();
    }
    
    const std::vector<MaterialTemplateItem> &channels = m_Template->GetTemplate();
    for (int i = 0; i < channels.size(); i ++ )
    {
        MaterialValueNodeEnum channelIndex = channels[i].channelIndex;
        if (channelIndex == MaterialValueNodeEnum::TINT)
        {
            MakeMaterialAttrib(material->tint, GetInputValueByKey((unsigned int)MaterialValueNodeEnum::TINT));
        }
        if (channelIndex == MaterialValueNodeEnum::DENSITY)
        {
            MakeMaterialAttrib(material->density, GetInputValueByKey((unsigned int)MaterialValueNodeEnum::DENSITY));
        }
    }
}