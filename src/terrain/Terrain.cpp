#include "Terrain.h"

Terrain::Terrain():Geometry(GeometryType::TERRAIN)
{
    TerrainCounter += 1;
    this->m_Name = "Terrain " + std::to_string(Terrain::TerrainCounter);
    // default meta;
    // generate vertices;

}
Terrain::Terrain(TerrainMeta meta):Geometry(GeometryType::TERRAIN)
{
    TerrainCounter += 1;
    this->m_Name = "Terrain " + std::to_string(Terrain::TerrainCounter);
    m_Meta = meta;
    // generate vertices;
}


Terrain::~Terrain()
{
    delete m_Shader;
    delete m_Dp;
}



void Terrain::GenIndices()
{
    unsigned int index = 0;
    unsigned int faceIndex = 0;
    for (unsigned int i = 0; i < m_Meta.res-1; i++)
    {
        for (unsigned int j = 0; j < m_Meta.res-1; j++)
        {
            //for (unsigned int k = 0; k < 2; k++)
            //{
                //m_Indices[index] = (j + m_Width * (i + k));
            //    index += 1;
            //}
            unsigned int indices[3];
            indices[0] = GetIndicesIndex(i, j);
            indices[1] = GetIndicesIndex(i + 1, j);
            indices[2] = GetIndicesIndex(i + 1, j + 1);
            m_Mesh.AppendFaceIndices(faceIndex, indices, 3);
            faceIndex += 1;

            indices[0] = GetIndicesIndex(i, j);
            indices[1] = GetIndicesIndex(i + 1, j + 1);
            indices[2] = GetIndicesIndex(i, j + 1);

            m_Mesh.AppendFaceIndices(faceIndex, indices, 3);
            index += 6; 
            faceIndex += 1;
        }
    }
    m_Mesh.CalculateFaceAreaPercentage();
    m_Mesh.CalculateIndices();
}

void Terrain::AttachMeta(TerrainMeta meta)
{
    m_Meta = meta;
}

unsigned int Terrain::GetIndicesIndex(int i, int j)
{
    return (i * m_Meta.res+ j);
}


void Terrain::InitProcessor()
{
    m_Dp = new TerrainDataProcessor();
    //m_Dp->SetDisplayMode(DisplayMode::WIREFRAME);
}


glm::vec3 Terrain::GetGridPoints(int x, int y)
{
    x = std::max(x, 0);
    y = std::max(y, 0);
    int index = y * m_Meta.res + x;
    return m_Mesh.GetPosition(index);
}

float Terrain::FitSize(float x)
{
    return (float) m_Meta.size / m_Meta.res * x;
}

void Terrain::PreRender()
{
//    Shader * s = this->GetMaterial()->GetShader();
//    s->setUniform1i("u_CombineNormal", 1);
    this->GetUniforms().Cache("u_CombineNormal", 1);

}
TerrainMeta & Terrain::GetTerrainMeta()
{
    return m_Meta;
}
int Terrain::GetTessUnit()
{
    return m_Unit;
}

bool Terrain::HasTess()
{
    return true;
}

void Terrain::ReadyToScene()
{
    m_Mats[0]->AddShaderSnippet(ShaderSnippet::TerrainSnippet(m_Meta.renderType == TerrainRenderType::GPU));
    Geometry::ReadyToScene();
    m_Mats[0]->GetShader()->Statistic();
}
int Terrain::TerrainCounter = 0;
