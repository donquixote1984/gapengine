#include "GeometryDataProcessor.h"

DataProcessor::~DataProcessor()
{}

void DataProcessor::Draw()
{
    for (int i = 0; i < m_RawVerticesClusterNum; i++)
    {
        m_RawVerticesCluster[i].Draw(m_DisplayMode);
    }
}

void DataProcessor::SetDisplayMode(DisplayMode mode)
{
    m_DisplayMode = mode;
}

DisplayMode DataProcessor::GetDisplayMode() const
{
    return m_DisplayMode;
}
 std::vector<Mesh> DataProcessor::GetProcessedData()
 {
    return m_Result;
 }
 
GeometryDataProcessor::~GeometryDataProcessor()
{}
void GeometryDataProcessor::Process(Geometry *g)
{
    //std::vector<Mesh> meshes = 
    GeometryData *gd = g->m_GeoData;
    m_Result = gd->GetMeshes();
    for (DataPostProcessing * dpp : m_Postprocessings)
    {

        m_Result = dpp ->Process(m_Result);
    }
    ProcessResult(m_Result, g);
}



void GeometryDataProcessor::AddPostProcessing(DataPostProcessing * dpp)
{
    if (dpp->GetType() == PostProcessingType::POINTONLY)
    {
        SetDisplayMode(DisplayMode::POINTS);
    }

    if (dpp->GetType() == PostProcessingType::SCATTER)
    {
        SetDisplayMode(DisplayMode::POINTS);
    }
    m_Postprocessings.push_back(dpp);
}

void GeometryDataProcessor::ProcessResult(std::vector<Mesh> meshes, Geometry *g)
{
    m_RawVerticesCluster = std::shared_ptr<RawVertices[]>(new RawVertices[meshes.size()]);
    m_RawVerticesClusterNum = (int)meshes.size();
    RawVertices * rawVerticesClusterArr = m_RawVerticesCluster.get();

    
    for (int i = 0; i < meshes.size(); i++)
    {
        Mesh mesh = meshes[i];
        rawVerticesClusterArr[i].Init(mesh.GetVertices(), mesh.GetVerticesCount());
        //rawVerticesClusterArr[i]
        if (mesh.HasIndices())
        {
            m_RawVerticesCluster[i].InitIndex(mesh.GetIndices(), mesh.GetFaceCount()* 3);
        }
        m_RawVerticesCluster[i].Buffer(mesh.GetLayout());

/*
        if (g->m_Instancing) 
        {
            std::vector<Mesh> instancedPoints = g->m_Gdp->GetProcessedData();
            //m_RawVerticesCluster[i].SetInstancing(true, g->m_InstanceCount);
            std::vector<glm::mat4> models;
            for (auto & rst: instancedPoints)
            {
                int verticesCount = rst.GetVerticesCount();
                glm::mat4 *models = new glm::mat4[verticesCount];
                for ( int j = 0; j < verticesCount; j++)
                {
                    glm::vec3 pos = rst.GetPosition(j);
                    glm::mat4 model;
                    model = glm::translate(model, pos);
                    //models.push_back(model);
                    models[j] = model;
                }
                //m_RawVerticesCluster[i].SetInstancingMVP(models);
            }
        }
*/
       // mesh.Log();
        
    }
}