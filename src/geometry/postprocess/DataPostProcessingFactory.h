#pragma once
#include "Scatter.h"
#include "Noise.h"
#include "PointOnly.h"
#include "Replica.h"
#include "DataPostProcessing.h"

class DataPostProcessingFactory
{
public:
    static DataPostProcessing *GetPostProcessing(PostProcessingType type)
    {
        if (type == PostProcessingType::NOISE)
        {
            return new Noise();
        }

        if (type == PostProcessingType::SCATTER)
        {
            return new Scatter();
        }

        if (type == PostProcessingType::POINTONLY)
        {
            return new PointOnly();
        }

        if (type == PostProcessingType::REPLICA)
        {
            return new Replica();
        }

        return nullptr;
    }
};