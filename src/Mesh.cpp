#include "Mesh.h"

Mesh::Mesh()
{}
Mesh::Mesh(int verticesCount, std::initializer_list<int> layouts): m_VerticesCount(verticesCount)
{
    for (int i: layouts)
    {
        m_Stride += i;
    }
    if (layouts.size() > 1)
    {
        m_HasNormal = true;
    }

    m_Vertices = std::shared_ptr<float []>(new float [verticesCount * m_Stride]);
    m_Layouts = util::ILToByte(layouts);
}

Mesh::Mesh(std::shared_ptr<float[]> data, int verticesCount, std::initializer_list<int> layouts):m_Vertices(data), m_VerticesCount(verticesCount)
{
    for (int i: layouts)
    {
        m_Stride += i;
    }
    if (layouts.size() > 1)
    {
        m_HasNormal = true;
    }
    //m_Vertices = data;
    m_Layouts = util::ILToByte(layouts);
}

Mesh::Mesh(std::shared_ptr<float[]> data, std::shared_ptr<int[]> indices, int verticesCount, int indiceCount, std::initializer_list<int> layouts):m_Vertices(data), m_VerticesCount(verticesCount)
{
    for (int i: layouts)
    {
        m_Stride += i;
    }
    if (layouts.size() > 3)
    {
        m_HasNormal = true;
    }
    m_HasIndices = true;
    m_HasFace = true;
    //m_Vertices = data;
    m_Layouts = util::ILToByte(layouts);
}


Mesh::Mesh(std::shared_ptr<float[]> data, int verticesCount, std::bitset<32>layouts):m_Vertices(data), m_VerticesCount(verticesCount)
{
    int p1 = (layouts      & std::bitset<32>{"00000111"}).to_ulong();
    int p2 = (layouts >> 3 & std::bitset<32>{"00000111"}).to_ulong();
    int p3 = (layouts >> 6 & std::bitset<32>{"00000111"}).to_ulong();
    m_Stride = p1 + p2 + p3;
    if (p2 > 0)
    {
        m_HasNormal =true;
    }
    //m_Vertices = std::shared_ptr<float[]>(new float [verticesCount * m_Stride]);
    m_Layouts = layouts;
}






 MeshMeta Mesh::GetMeta()
 {
    return m_Meta;
 }


void Mesh::AddVertex(std::array<float, 22> v)
{
    float * vertices = m_Vertices.get();
    //Logger::GetLogger()->log(vertices, 8, "[Vertex]");
    std::memcpy(vertices + m_VerticesAdded * m_Stride, v.data(), m_Stride * sizeof(float));
    m_VerticesAdded += 1;
}

void Mesh::InitFace(int faceCount)
{
    if (faceCount > 0) 
    {
        m_HasFace = true;
    }
    m_HasIndices = true;
    m_HasNormal = true;
    m_FacesCount = faceCount;
    m_Faces = std::shared_ptr<Face[]>(new Face[faceCount]);
    m_Meta.facesAreaList = std::shared_ptr<float[]>(new float[faceCount]);
}



Vertex * Mesh::GetVertex8f()
{
    float * vertices = m_Vertices.get();
    return (Vertex *) vertices;
}

glm::vec3 Mesh::GetPosition(int indice)
{
    float * vertices = m_Vertices.get();
    int dimension = (m_Layouts & std::bitset<32>{"00000111"}).to_ulong();
    float p0 = vertices[indice * m_Stride];
    float p1 = vertices[indice * m_Stride + 1];
    float p2 = 0;
    if (dimension ==3)
    {
        p2 = vertices[indice * m_Stride + 2];
    }

    return glm::vec3(p0, p1, p2);
}

float * Mesh::GetPositionPtr(int indice)
{
    float * vertices = m_Vertices.get();
    return vertices + indice * m_Stride;
}

Vertex Mesh::GetPositionAndNormal(int indice)
{
    Vertex v;
    
    float * vertices = m_Vertices.get();
    int pDimension = (m_Layouts & std::bitset<32>{"0000000000000111"}).to_ulong();
    int nDimension = (m_Layouts & std::bitset<32>{"0000000000111000"}).to_ulong();

    v.position.x= vertices[indice * m_Stride];
    v.position.y= vertices[indice * m_Stride + 1];
    if (pDimension == 3) {
        v.position.z= vertices[indice * m_Stride + 2];
    } else {
        v.position.z= 0;
    }

    if (nDimension == 0)
    {
        v.normal = glm::vec3(0);
    }
    else 
    {
        v.normal.x= vertices[indice * m_Stride + 3];
        v.normal.y= vertices[indice * m_Stride + 4];

        if (nDimension == 3)
        {
            v.normal.z = vertices[indice * m_Stride + 5];
        } else {
            v.normal.z = 0;
        }
    }

    return v;
}


Face * Mesh::GetFaces()
{
    return m_Faces.get();
}

void Mesh::AppendFaceIndices(unsigned int faceIndex, unsigned int * indices, unsigned int indicesCount)
{
    m_IndicesCount += indicesCount;
    Face * faces = GetFaces();
    faces[faceIndex].indices = std::shared_ptr<int []>(new int[indicesCount]);
    int * indice_ptr = faces[faceIndex].indices.get();
    faces[faceIndex].indicesCount = indicesCount;
    for (int i = 0 ; i < indicesCount; i++)
    {
        indice_ptr[i] = indices[i];
    }

    if (indicesCount > 2)
    {
        int ind0 = indices[0];
        int ind1 = indices[1];
        int ind2 = indices[2];

        faces[faceIndex].area = util::TriArea(
            GetPosition(ind0),
            GetPosition(ind1),
            GetPosition(ind2)
        );
       //  Logger::GetLogger()->log(std::to_string(faces[faceIndex].area));
        m_TotalArea += faces[faceIndex].area;
    }
}

void Mesh::CalculateFaceAreaPercentage()
{
    if (!m_HasFace)
    {
        return;
    }
    Face * faces = GetFaces();
    float totalPercentage = 0;
    for (int i = 0; i < m_FacesCount; i++)
    {
        faces[i].areaPercentage = faces[i].area / m_TotalArea;
        totalPercentage += faces[i].areaPercentage;
        m_Meta.facesAreaList[i] = totalPercentage;
    }
}
void Mesh::CalculateIndices()
{
    if (m_HasFace && m_HasIndices)
    {
        m_Indices = std::shared_ptr<int []>(new int[m_IndicesCount]);
        int * indicesPtr = m_Indices.get();
        int indicesCursor = 0;
        Face * faces = GetFaces();
        for (int j = 0; j < m_FacesCount; j++)
        {
            auto & face = faces[j];
            int * faceIndices = face.indices.get();
            std::copy(faceIndices, faceIndices + face.indicesCount, indicesPtr + indicesCursor);
            indicesCursor += face.indicesCount;
        }
    }
}

int Mesh::GetFacesCount()
{
    return m_FacesCount;
}

bool Mesh::HasNormal()
{
    return m_HasNormal;
}
void Mesh::WithNormal(bool withNormal)
{
    m_HasNormal = withNormal;
}

float * Mesh::GetVertices()
{
    return m_Vertices.get();
}
int * Mesh::GetIndices()
{
    return m_Indices.get();
}
std::bitset<32> Mesh::GetLayout()
{
    return m_Layouts;
}

int Mesh::GetVerticesCount()
{
    return m_VerticesCount;
}

bool Mesh::HasIndices()
{
    return m_HasIndices;
}

int Mesh::GetFaceCount()
{
    return m_FacesCount;
}

std::string Mesh::ToString()
{
    std::stringstream ss;
    ss << "Layouts:" << m_Layouts << std::endl;
    ss << "vertices count : " << m_VerticesCount << std::endl;
    ss << "Faces count : " << m_FacesCount << std::endl;
    ss << "Indice count : " << m_IndicesCount << std::endl;

    ss << "Total Area: " << m_TotalArea << std::endl;
    ss << "Vertices: ";
    for (int i = 0; i < m_VerticesCount * m_Stride; i++)
    {
        if (i % m_Stride == 0)
        {
            ss << std::endl;
        }
        ss << (m_Vertices[i] < 0.001 ? 0 : m_Vertices[i]) << ", ";
       
    }
    ss << std::endl;
    ss << "Indices: " << std::endl;
    for (int i = 0; i < m_IndicesCount; i++)
    {
        ss << m_Indices[i] << ", ";
    }
    return ss.str();
}

void Mesh::DumpToFile()
{
    std::ofstream outfile;
    outfile.open(m_Name + ".mesh.txt", std::ios::out);

    outfile << ToString();
    outfile.flush();
    outfile.close();
}

void Mesh::SetName(const std::string &name)
{
    m_Name = name;
}

void Mesh::Log()
{
    Logger::GetLogger()->log(ToString());
}
int Mesh::GetLayoutSize()
{
    return util::GetLayoutSize(m_Layouts);
}
void Mesh::RebuildUV()
{
   
}

int Mesh::GetStride()
{
    return m_Stride;
}


void Mesh::AddTextures(MeshTexture texture)
{
    m_Material.valid = true;
    m_Material.textures.push_back(texture);
}
std::vector<MeshTexture> Mesh::GetTextures()
{
    return m_Material.textures;
}

void Mesh::UpdateBoneData(int indices, unsigned int boneId, float weight)
{
    m_HasBone = true;
    float * ptr = GetPositionPtr(indices);
    ptr +=  (3 + 3 + 2 + 3 + 3);// to bone segment
    
    for (int i = 0; i < constants::MAX_BONE_PER_VERTEX; i++)
    {
        if (*ptr < 0.0f)
        {
            *ptr = (float) boneId;
            *(ptr + 4) = weight;
            return;
        }
        ptr += 1;
    }
}

void Mesh::AddVertexWeight(std::unordered_map<std::string, Bone *> boneMap)
{
    for (auto &entry: boneMap)
    {
        Bone * b = entry.second;
        auto weights = b->GetWeights();

        for (auto &kv : weights)
        {
            //float ptr = GetPositionPtr(kv.first)
            UpdateBoneData(kv.first, b->GetId(), kv.second);
        }
    }
}

bool Mesh::HasBone()
{
    return m_HasBone;
}

bool Mesh::HasMaterial()
{
    return m_Material.valid;
}

MeshMaterial& Mesh::GetMaterial()
{
    return m_Material;
}
