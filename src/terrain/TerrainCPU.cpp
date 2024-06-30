#include "TerrainCPU.h"

TerrainCPU::TerrainCPU(TerrainMeta meta): Terrain(meta)
{}

void TerrainCPU::GenTerrainMesh()
{

    stbi_set_flip_vertically_on_load(true);
    int width, height, nChannels;
    unsigned char *data = stbi_load(m_Meta.heightmap.c_str(), &width, &height, &nChannels, 4);
    int res = m_Meta.res;
    int verticesCount = res * res;
    int indicesCount = (res-1) * (res-1) * 6;
    int faceCount = (res - 1) * (res - 1) * 2;
    //int indices[(res-1) * (res-1) * 6];  // can not alloc 1k * 1k *6 size in stack.

    unsigned int index = 0;

    m_Mesh = Mesh(res * res, {3,3,2,3,3});
    //InitGeometryData(mesh, m_Meta.res * m_Meta.res, {3,3,2,3,3});
    //GeometryDataFactory::CreateGeometryData(mesh);
    m_Mesh.InitFace(faceCount);

    for (unsigned int j = 0; j < res; j++)
    {
        for (unsigned int i = 0; i < res; i++)
        {
            std::array<float, 22> vertex;
            //unsigned char* texel = data + (j + m_Width * i) *m_NChannels;
            //unsigned char  y = texel[0];
            vertex[0] =  FitSize(i - res /  2.0) ; //-m_Height/2.0f + m_Height*i/(float)m_Height;

            vertex[1] = SampleHeight(data,i, j, width, height, nChannels) * 0.02;

            vertex[2] = FitSize(j - res /2.0);

            // normal
            glm::vec3 norm = glm::normalize(GetNormal(i, j));
            vertex[3] = norm.x;
            vertex[4] = norm.y;
            vertex[5] = norm.z;

            // uv
            vertex[6] = (float)i / res;
            vertex[7] = (float) j / res;

            glm::vec3 tangent = glm::vec3(0); 
            glm::vec3 bitangent = glm::vec3(0);
            GetTangent(i, j, tangent, bitangent);
            // tangent
            vertex[8] = tangent.x;
            vertex[9] = tangent.y;
            vertex[10] = tangent.z;
            // bitangent
            vertex[11] = bitangent.x;
            vertex[12] = bitangent.y;
            vertex[13] = bitangent.z;
            //
            glm::vec3 renormal = - normalize(glm::cross(tangent, bitangent));
            vertex[3] = renormal.x;
            vertex[4] = renormal.y;
            vertex[5] = renormal.z;

            vertex[14] = -1.0f;
            vertex[15] = -1.0f;
            vertex[16] = -1.0f;
            vertex[17] = -1.0f;

            vertex[18] = -0.0f;
            vertex[19] = -0.0f;
            vertex[20] = -0.0f;
            vertex[21] = -0.0f;
            m_Mesh.AddVertex(vertex);
            index += 14;
        }
    }
    stbi_image_free(data);
}

glm::vec3 TerrainCPU::GetNormal(int x, int y)
{
    glm::vec3 v1 = GetGridPoints(x, y);
    glm::vec3 v2 = GetGridPoints(x-1, y);
    glm::vec3 v3 = GetGridPoints(x, y-1);

    return glm::cross((v2- v1), (v1-v3));
}

void TerrainCPU::GetTangent(int x, int y, glm::vec3 &tangent, glm::vec3 &bitangent)
{
    glm::vec3 v1 = GetGridPoints(x-1, y);
    glm::vec2 uv1 = {(float)(x - 1) / m_Meta.res,  (float)y / m_Meta.res};

    glm::vec3 v2 = GetGridPoints(x-1, y-1);
    glm::vec2 uv2 = {(float)(x - 1) / m_Meta.res, (float) (y - 1)/m_Meta.res };

    glm::vec3 v3 = GetGridPoints(x, y-1);
    glm::vec2 uv3 = {(float)(x) / m_Meta.res, (float) (y - 1) / m_Meta.res };

    glm::vec3 v4 = GetGridPoints(x, y);
    glm::vec2 uv4 = {(float)(x) / m_Meta.res, (float) (y) / m_Meta.res };

    glm::vec3 edge1 = v2 - v1;
    glm::vec3 edge2 = v3 - v1;

    glm::vec2 deltaUV1 = uv2 - uv1;
    glm::vec2 deltaUV2 = uv3 - uv1;

    GLfloat f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

    tangent.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
    tangent.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
    tangent.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
    tangent = glm::normalize(tangent);

    bitangent.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
    bitangent.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
    bitangent.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);
    bitangent = glm::normalize(bitangent);  
}
unsigned char TerrainCPU::SampleHeight(unsigned char *data, int i, int j,  int texWidth, int texHeight, int channels)
{
    int coordx = floor(i * ((float)texWidth / m_Meta.res));//floor((float)i / m_Meta.res * texWidth);
    int coordy = floor(j * ((float)texHeight / m_Meta.res));
    unsigned char* texel = data + (coordx + coordy* texWidth) *channels;
   
    return texel[0];
}



void TerrainCPU::ProcessData()
{
    GenTerrainMesh();
    GenIndices();
    GeometryData *d = GeometryDataFactory::CreateGeometryData(m_Mesh);
    InitGeometryData(d, 1);
    m_Dp->Process(this);
}


void TerrainCPU::PreRender()
{
    Shader * s = this->GetMaterial()->GetShader();
    s->setUniform1i("u_CombineNormal", 1);
}

