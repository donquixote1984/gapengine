#include "TerrainSDF.h"

TerrainSDF::TerrainSDF(VolumeMeta meta): Volume(meta)
{
    m_TerrainNoise = new Texture("res/textures/noise/raymarchterrain.png", false, true);
}

TerrainSDF::~TerrainSDF()
{
    delete m_TerrainNoise;
}
void TerrainSDF::PreRender()
{
    //Shader *s = this->GetMaterial()->GetShader();
    //m_TerrainNoise->Bind(0);
    this->GetUniforms().Cache("u_TerrainNoiseTexture", 0);
    this->GetUniforms().Cache("u_Resolution", m_CanvasWidth, m_CanvasHeight);
    glViewport(0, 0, m_CanvasWidth, m_CanvasHeight);
}