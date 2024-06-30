#include "TerrainDataProcessor.h"
#include "Terrain.h"

TerrainDataProcessor::~TerrainDataProcessor()
{

}
void TerrainDataProcessor::Process(Geometry * g)
{
    Terrain *t = static_cast<Terrain *>(g);
    std::vector<Mesh> meshes = t->GetGeoData()->GetMeshes();
    m_RawVerticesCluster = std::shared_ptr<RawVertices[]>(new RawVertices[meshes.size()]);
    m_RawVerticesClusterNum = meshes.size();
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

        if (t->GetTerrainMeta().renderType == TerrainRenderType::GPU)
        {
            
            m_RawVerticesCluster[i].EnableTessellation(t->GetTessUnit());
        }
    }
}