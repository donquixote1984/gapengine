#pragma once
#include <memory>
#include "../geometry/Geometry.h"
#include "../geometry/GeometryDataProcessor.h"


class TerrainDataProcessor: public DataProcessor
{
public:
    ~TerrainDataProcessor();
    void Process(Geometry * g) override;
};