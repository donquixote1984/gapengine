#pragma
#include "../../Mesh.h"
#include "DataPostProcessing.h"
#include <memory>
class Replica: public DataPostProcessing
{
public:
    std::vector<Mesh> Process(std::vector<Mesh> ppu) override;
    PostProcessingType GetType() override;
};