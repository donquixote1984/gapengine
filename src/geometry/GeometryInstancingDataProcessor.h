#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "../alg/Algorithm.h"
#include "GeometryDataProcessor.h"
#include "RawInstancingVertices.h"
#include "GeometryInstancing.h"
#include "../exceptions/MissingConfigurationException.h"
#include <memory>
#include <vector>
#include "../Mesh.h"
#include "../Transform.h"
#include "generator/PointsGenerator.h"

class GeometryInstancing;
class GeometryDataProcessor;
class GeometryInstancingDataProcessor: public GeometryDataProcessor
{
private:
    GeometryDataProcessor *unused = nullptr;
protected:
    std::shared_ptr<RawInstancingVertices[]> m_RawVerticesCluster;
    glm::mat4 * instancingMVPs;
    void ProcessResult(std::vector<Mesh> meshes, Geometry *g) override;
public:
    int GetInstancingCount(GeometryInstancing * g) const;
    std::shared_ptr<glm::mat4 []> GetInstancingMVP(GeometryInstancing * g) const;
};