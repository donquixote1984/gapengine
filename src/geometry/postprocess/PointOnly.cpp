#include "PointOnly.h"

std::vector<Mesh> PointOnly::Process(std::vector<Mesh> ppu)
{
    return ppu;
}

PostProcessingType PointOnly::GetType()
{
    return PostProcessingType::POINTONLY;
}