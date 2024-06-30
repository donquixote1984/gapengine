#pragma once
#include <glm/glm.hpp>
#include <array>
#include <time.h>
#include <memory>
#include <cmath>
#include "../Mesh.h"
#include "../Vertex.h"
#include "AlgUtil.h"

namespace alg
{
//  can not tell how to use interval tree, use bisearch. https://stackoverflow.com/questions/66510570/algorithm-for-finding-ranges-which-include-a-number
// use this impl: https://medium.com/@daviddelaiglesiacastro/3d-point-cloud-generation-from-3d-triangular-mesh-bbb602ecf238
    Mesh Scatter(Mesh mesh, int n, int seed = 1);
    
};