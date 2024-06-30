#include "GeometryMaterialValueNode.h"
GeometryMaterialValueNode::GeometryMaterialValueNode(MaterialTemplate *mt): MaterialValueNode(mt)
{}
void GeometryMaterialValueNode::Calc()
{
    BasicMaterial * material = nullptr;
    if (m_Template->HasVariation(MaterialTemplateVariation::PBR))
    {
        material = GetOrCreateMaterial<PBRMaterial>();
    } else if (m_Template->HasVariation(MaterialTemplateVariation::TERRAIN)) {
        material = GetOrCreateMaterial<TerrainMaterial>();
    } else {
        material = GetOrCreateMaterial<DefaultMaterial>();
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

        if (channelIndex == MaterialValueNodeEnum::NORMAL)
        {
            MakeMaterialAttrib(material->normal, GetInputValueByKey((unsigned int)MaterialValueNodeEnum::NORMAL));
        }

        if (channelIndex == MaterialValueNodeEnum::SHININESS)
        {
            MakeMaterialAttrib(material->shininess, GetInputValueByKey((unsigned int)MaterialValueNodeEnum::SHININESS));
        }
        if (channelIndex == MaterialValueNodeEnum::ROUGHNESS)
        {
            MakeMaterialAttrib(material->roughness, GetInputValueByKey((unsigned int)MaterialValueNodeEnum::ROUGHNESS));
        }
        if (channelIndex == MaterialValueNodeEnum::METALNESS)
        {
            MakeMaterialAttrib(material->metalness, GetInputValueByKey((unsigned int)MaterialValueNodeEnum::METALNESS));
        }

        if (channelIndex == MaterialValueNodeEnum::TILE)
        {
            MakeMaterialAttrib(material->tile, GetInputValueByKey((unsigned int)MaterialValueNodeEnum::TILE));
            if (material->tile.constant > 0)
            {
                material->SetTilable(true);
            }
        }
    }
}