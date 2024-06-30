#include "Scatter.h"
std::vector<Mesh> Scatter::Process(std::vector<Mesh> meshes)
{
    std::vector<Mesh> result;
    for (auto & mesh: meshes)
    {
        Mesh pc = alg::Scatter(mesh, 10000, 1);
        result.push_back(pc);
    }
    //std::transform(ppu.begin(), ppu.end(), )
    return result;
}

PostProcessingType Scatter::GetType()
{
    return PostProcessingType::SCATTER;
}