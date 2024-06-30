#include "Replica.h"
std::vector<Mesh> Replica::Process(std::vector<Mesh> meshes)
{
    return meshes;
}
PostProcessingType Replica::GetType()
{
    return PostProcessingType::REPLICA;
}