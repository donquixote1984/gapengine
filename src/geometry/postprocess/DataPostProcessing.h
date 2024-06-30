#pragma once
#include <vector>
#include "../../Mesh.h"

enum class PostProcessingType
{
    SCATTER,
    REPLICA,
    POINTONLY,
    NOISE,
    NONE
};

class DataPostProcessing
{
public:
    virtual std::vector<Mesh> Process(std::vector<Mesh> ppu) = 0;
    virtual PostProcessingType GetType() = 0;
};