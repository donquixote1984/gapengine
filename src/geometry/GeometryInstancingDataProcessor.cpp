#include "GeometryInstancingDataProcessor.h"

void GeometryInstancingDataProcessor::ProcessResult(std::vector<Mesh> meshes, Geometry *g)
{
    //m_Mat->AddShaderSnippet(ShaderSnippet::InstancingSnippet(offset));

    GeometryInstancing *gi = (GeometryInstancing *) g;
    int instancingCount = GetInstancingCount(gi);
    gi->SetInstancingCount(instancingCount);
    auto meta = gi->GetMeta();
    int count = meta.instancing.count;

    std::shared_ptr<glm::mat4[]> transforms = GetInstancingMVP(gi);

    m_RawVerticesCluster = std::shared_ptr<RawInstancingVertices[]>(new RawInstancingVertices[meshes.size()]);
    m_RawVerticesClusterNum = meshes.size();
    RawInstancingVertices * rawVerticesClusterArr = (RawInstancingVertices*)m_RawVerticesCluster.get();

    
     for (int i = 0; i < meshes.size(); i++)
    {
        Mesh mesh = meshes[i];
        rawVerticesClusterArr[i].Init(mesh.GetVertices(), mesh.GetVerticesCount());
        //rawVerticesClusterArr[i]
        if (mesh.HasIndices())
        {
            rawVerticesClusterArr[i].InitIndex(mesh.GetIndices(), mesh.GetFaceCount()* 3);
        }
        rawVerticesClusterArr[i].Buffer(mesh.GetLayout());
//        GeometryInstancingType instancingType = gi->GetInstancingType();

        rawVerticesClusterArr[i].SetInstancing(transforms.get(), instancingCount);
    }
}

int GeometryInstancingDataProcessor::GetInstancingCount(GeometryInstancing * g) const
{
    auto & meta = g->GetMeta();
    if (meta.instancing.type == (unsigned int)GeometryInstancingType::INHERIT)
    {
        if (g->GetParent() == nullptr)
        {
            throw MissingConfigurationException("Geometry use 'inherit' instancing but have no parent geometry.");
        }
        //count = g->GetParent()->GetVerticesCount();
    } 
    if (meta.instancing.distribution == (unsigned int)PointGeneratorDistributionType::GRID)
    {
        return meta.instancing.rows * meta.instancing.columns;
    }
    return meta.instancing.count;
}


std::shared_ptr<glm::mat4[]> GeometryInstancingDataProcessor::GetInstancingMVP(GeometryInstancing * g) const
{
    // if it is static,  gen points from pointgenerator,  otherwise use parent scatter point.

    auto &meta = g->GetMeta();
    GeometryInstancingType instancingType = (GeometryInstancingType)meta.instancing.type;
    if ( instancingType == GeometryInstancingType::INHERIT)
    {
        // aquire parent points.
       // int count = GetInstancingCount(g);
        unsigned int count = meta.instancing.count;
        std::shared_ptr<glm::mat4 []> mvps = std::shared_ptr<glm::mat4 []>(new glm::mat4[count]);
        //glm::mat4 postTrans = g->GetParent()->GetModelMat();
        Geometry * parentGeometry  = static_cast<Geometry *>(g->GetParent());
        for(Mesh & m : parentGeometry->GetGeoData()->GetMeshes())
        {
            Mesh scattered = alg::Scatter(m, count);
            //glm::vec3 pos = scattered.GetPosition(i);
            int vCount = scattered.GetVerticesCount();
            for (int i = 0; i < vCount; i++)
            {
                glm::vec3 pos = scattered.GetPosition(i);
                //glm::mat4 model = glm::mat4(1.0f);
                //model = glm::translate(model, pos);
              //  model = model * postTrans;
                Transform t = pos;
                mvps[i] = t.ToMatrix4(meta.instancing.scaleVariant);
            }
        }

//        std::vector<Mesh> meshes = g->GetParent()->GetDataProcessor()->GetProcessedData();
        //return mvps.get();
        return mvps;
    }
    else 
    {
        unsigned int count = meta.instancing.count;
        PointGeneratorDistributionType distribution = (PointGeneratorDistributionType)meta.instancing.distribution;
        PointsGenerator pg(count, glm::vec3(0.0f));
        pg.SetScaleVariant(meta.instancing.scaleVariant);
        if (distribution == PointGeneratorDistributionType::CIRCLE)
        {
            return pg.CircleMat(meta.instancing.radius);
        } else if (distribution == PointGeneratorDistributionType::GRID) {
            return pg.GridMat(meta.instancing.rows, meta.instancing.columns, meta.instancing.interval, meta.instancing.direction.up, meta.instancing.direction.forward, meta.instancing.offset);
        } else {
            return pg.SquareMat(meta.instancing.radius);
        }
    }
}

void GeometryInstancingDataProcessor::Draw()
{
    for (int i = 0; i < m_RawVerticesClusterNum; i++)
    {
        m_RawVerticesCluster[i].Draw(m_DisplayMode);
    }
}