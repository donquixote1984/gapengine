#include "GeometryData.h"

GeometryData::GeometryData() {}

bool GeometryData::IsEmpty()
{
    return m_IsEmpty;
}

GeometryData::GeometryData(std::vector<Mesh> meshes):m_IsEmpty(false)
{
    FeedData(meshes);
}

GeometryData::GeometryData(std::vector<Mesh> meshes, std::vector<std::unordered_map<std::string, Bone *>> bones): m_IsEmpty(false)
{
    FeedData(meshes);
    FeedData(bones);
}

GeometryData::GeometryData(Mesh mesh): m_IsEmpty(false)
{
    FeedData(mesh);
}

GeometryData::GeometryData(float *rawArr, int vertexCount, std::initializer_list<int> layouts):m_IsEmpty(false)
{
    FeedData(rawArr, vertexCount, layouts);
}
GeometryData::GeometryData(std::shared_ptr<float []> rawArrSP, int vertexCount, std::initializer_list<int> layouts):m_IsEmpty(false)
{
    FeedData(rawArrSP, vertexCount, layouts);
}

void GeometryData::FeedData(Mesh mesh)
{
    m_Meshes.clear();
    m_Meshes.push_back(mesh);
}

void GeometryData::FeedData(std::vector<std::unordered_map<std::string, Bone *>> bones)
{
    m_IsEmpty = false;
    m_Bones = bones;
}
void GeometryData::FeedData(std::vector<Mesh> meshes)
{
    m_IsEmpty = false;
    m_Meshes = meshes;
}
void GeometryData::FeedData(float *rawArr, int vertexCount, std::initializer_list<int> layouts)
{
    m_IsEmpty = false;
    std::bitset<32> bLayout = util::ILToByte(layouts);
    Mesh m(std::shared_ptr<float[]>(rawArr), vertexCount, bLayout);
    m_Meshes.push_back(m);
}

void GeometryData::FeedData(std::shared_ptr<float[]> rawArrSp, int vertexCount, std::initializer_list<int> layouts)
{
    m_IsEmpty = false;
    std::bitset<32> bLayout = util::ILToByte(layouts);
    Mesh m(rawArrSp, vertexCount, bLayout);
    m_Meshes.push_back(m);
}

int GeometryData::GetLayoutSize()
{
    if (m_Meshes.size() == 0)
    {
        return 0;
    }
    return m_Meshes[0].GetLayoutSize();
}
bool GeometryData::HasBone()
{
    if (m_Meshes.size() > 0) 
    {
        return m_Meshes[0].HasBone();
    }
    return false;
}


/*
void MeshGeometryData::InitWithData(std::vector<Mesh> meshes, std::byte layout)
{
    m_Meshes = meshes;
    int size = m_Meshes.size();
    m_RawVerticesCluster = new RawVertices[size];
    m_RawVerticesClusterNum = size;
    m_Type = GeometryDataType::MESHES;
    for (uint i = 0; i < size; i++)
    {
        //RawVertices r;
        int indices[m_Meshes[i].indicesCount];

        int indicesCursor = 0;
        for (int j = 0; j < m_Meshes[i].facesCount; j++)
        {
            auto & face = m_Meshes[i].faces[j];
            std::copy(face.indices, face.indices + face.indicesCount, indices + indicesCursor);
            indicesCursor += face.indicesCount;
        }

        m_RawVerticesCluster[i].Init(&(m_Meshes[i].vertices[0].position.x), m_Meshes[i].verticesCount);
        m_RawVerticesCluster[i].InitIndex(indices, m_Meshes[i].facesCount * 3);
        m_RawVerticesCluster[i].Buffer(layout);
       // m_RawVertices.push_back(r);
    }
}
*/
/*
void PointCloudGeometryData::InitWithData(std::vector<Mesh> meshes, std::byte layout)
{
    m_Type = GeometryDataType::POINTCLOUD;
    int size = meshes.size();
    m_RawVerticesCluster = new RawVertices[size];
    m_RawVerticesClusterNum = size;

    for (int i = 0; i < meshes.size(); i++)
    {
        PointCloud pc = alg::Scatter(meshes[i], 1000);
        Vertex * vertices = pc.vertices.get();
        m_RawVerticesCluster[i].Init(&(vertices[0].position.x), 1000);
        m_RawVerticesCluster[i].Buffer(layout);
    }
}
*/



GeometryData::~GeometryData()
{
    for (auto &m : m_Meshes)
    {
        /*
        delete [] m.vertices;
        for (int i = 0; i < m.facesCount; i++)
        {
            delete [] m.faces[i].indices;
        }

        delete [] m.faces;
        delete [] m.meta.facesAreaArr;
        */
    }

    for (auto &boneMap: m_Bones)
    {
        for (auto &kv : boneMap)
        {
            delete kv.second;
            kv.second = nullptr;
        }
    }
    m_Meshes.clear();
    m_Bones.clear();

    for (auto &ani: m_SceneAnimations)
    {
        delete ani;
        ani = nullptr;
    }
    m_SceneAnimations.clear();
}


std::vector<Mesh> GeometryData::GetMeshes()
{
    return m_Meshes;
}

void GeometryData::FeedSceneAnimation(std::vector<SceneAnimation *> animations)
{
    m_SceneAnimations  = animations;
    for (auto &sceneAnimation: m_SceneAnimations)
    {
        for (auto &kv: sceneAnimation->GetBoneAnimations())
        {
            std::string boneName = kv.first;
            BoneAnimation * bAnimation = kv.second;

            Bone * b = FindBone(boneName);
            if (b)
            {
                bAnimation->SetBone(b);
            }
        }
    }
}


SceneAnimation * GeometryData::GetSceneAnimation(unsigned int index)
{
    return m_SceneAnimations[index];
}

Bone* GeometryData::FindBone(const std::string &name)
{
    for (auto &boneMap: m_Bones)
    {
        if (boneMap.find(name) != boneMap.end())
        {
            return boneMap[name];
        }
    }
    return nullptr;
}


bool GeometryData::HasAnimation()
{
    return m_SceneAnimations.size() > 0;
}