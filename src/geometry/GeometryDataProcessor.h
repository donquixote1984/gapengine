#pragma once
#include <vector>
#include <memory>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "RawVertices.h"
#include "../Mesh.h"
#include "Geometry.h"
#include "postprocess/DataPostProcessing.h"
#include "DisplayMode.h"
class Mesh;
class Geometry; 

class DataProcessor
{
protected:
    std::shared_ptr<RawVertices[]> m_RawVerticesCluster;
    int m_RawVerticesClusterNum;
    DisplayMode m_DisplayMode = DisplayMode::MESH;
    std::vector<Mesh> m_Result;
public:
    virtual void Process(Geometry *g) = 0;
    virtual ~DataProcessor();
    void SetDisplayMode(DisplayMode mode);
    DisplayMode GetDisplayMode() const;
    std::vector<Mesh> GetProcessedData();
    RawVertices* GetRawVertices(int& clusterNum);
};

class GeometryDataProcessor: public DataProcessor
{
protected:
    std::vector<DataPostProcessing*> m_Postprocessings;
    virtual void ProcessResult(std::vector<Mesh> ppu, Geometry * g);
public:
    void Process(Geometry *g);
    ~GeometryDataProcessor();
    void AddPostProcessing(DataPostProcessing *dpp);
};