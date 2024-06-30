#pragma
#include "../../alg/Algorithm.h"
#include "../../Mesh.h"
#include "DataPostProcessing.h"
#include <memory>
class Scatter: public DataPostProcessing
{
public:
    std::vector<Mesh> Process(std::vector<Mesh> ppu) override;
    PostProcessingType GetType() override;
};