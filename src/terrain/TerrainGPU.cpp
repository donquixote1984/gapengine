#include "TerrainGPU.h"
#include "../render/passes/RenderPass.h"
#include "../render/passes/RenderPassLinkList.h"
TerrainGPU::TerrainGPU(TerrainMeta meta): Terrain(meta)
{
    if (!meta.useNoise) {
        m_Texture = new Texture(meta.heightmap);
    }
}

TerrainGPU::~TerrainGPU()
{
    delete m_Texture;
}

void TerrainGPU::GenTerrainMesh()
{
    int res = m_Meta.res;
    int verticesCount = res * res;
    int indicesCount = (res-1) * (res-1) * 6;
    int faceCount = (res - 1) * (res - 1) * 2;
    //int indices[(res-1) * (res-1) * 6];  // can not alloc 1k * 1k *6 size in stack.

    m_Mesh = Mesh(res * res * m_Unit, {3, 3, 2, 3, 3});
    m_Mesh.InitFace(faceCount);
    for (unsigned int j = 0; j < res; j++)
    {
        for (unsigned int i = 0; i < res; i++)
        {
            std::array<float, 22> vertex;
            //unsigned char* texel = data + (j + m_Width * i) *m_NChannels;
            //unsigned char  y = texel[0];
            for (int offsety = 0; offsety < 2; offsety++)
            {
                for (int offsetx = 0; offsetx < 2; offsetx ++)
                {
                    vertex[0] = FitSize(i + offsetx - res / 2.0) ; 
                    vertex[1] = 0.0f;
                    vertex[2] = FitSize(j + offsety - res /2.0);
            // normal
                    vertex[3] = 0.0f;
                    vertex[4] = 1.0f;
                    vertex[5] = 0.0f;
            // uv
                    vertex[6] = (float) (i + offsetx) / res;
                    vertex[7] = (float) (j + offsety) / res; 
            // tan / bitan
                    vertex[8] = .0f;
                    vertex[9] = .0f;
                    vertex[10] = .0f;
                    vertex[11] = .0f;
                    vertex[12] = .0f;
                    vertex[13] = .0f;

                    vertex[14] = -1.0f;
                    vertex[15] = -1.0f;
                    vertex[16] = -1.0f;
                    vertex[17] = -1.0f;

                    vertex[18] = -0.0f;
                    vertex[19] = -0.0f;
                    vertex[20] = -0.0f;
                    vertex[21] = -0.0f;
                    //m_Mesh.AddVertex(vertex);
                }
               
            }
            
        }
    }
}

void TerrainGPU::ProcessData()
{
    glPatchParameteri(GL_PATCH_VERTICES, m_Unit);
    GenTerrainMesh();
    GenIndices();
    GeometryData *d = GeometryDataFactory::CreateGeometryData(m_Mesh);
    InitGeometryData(d, 1);
    m_Dp->Process(this);
    //m_Dp->SetDisplayMode(DisplayMode::WIREFRAME);
}

void TerrainGPU::PreRender()
{
    Shader * s = this->GetMaterial()->GetShader();
    s->Bind();
    s->setUniform1i("u_CombineNormal", 1);
    if (!m_Meta.useNoise) 
    {
        m_Texture->Bind(constants::HEIGHT_MAP);
    }
    s->setUniform1i("u_HeightMap", constants::HEIGHT_MAP);
    s->setUniform4f("u_MapRange", m_Meta.range.x, m_Meta.range.y, m_Meta.range.z, m_Meta.range.w);

   // m_RenderPasses->First()
}

void TerrainGPU::InitPasses()
{
    Geometry::InitPasses();
    if (m_Meta.useNoise)
    {
        RenderPass *nrp = RenderPassLinkList::CreateRenderPass(RenderPassType::NOISE, this);
        nrp->Buffer(m_Meta.animated.x, 0);
        nrp->Buffer(m_Meta.animated.y, 1);
        nrp->Buffer(16, 2);
        m_RenderPasses->PrependRenderPass(nrp);
    } 
    //m_RenderPasses = new RenderPassLinkList();
    //m_RenderPasses->AppendRenderPass(RenderPassLinkList::CreateRenderPass(RenderPassType::NOISE));
}

