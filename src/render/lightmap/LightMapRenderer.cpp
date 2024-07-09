#include "LightMapRenderer.h"

LightMapRenderer::LightMapRenderer()
{
   m_PointLightMapArray = new PointLightMapArray();
   m_DirectionalLightMapArray = new DirectionalLightMapArray();
   m_SunLightMap = new SunLightMapCSM();
   //m_DirectionalLightMapArray->EnableDebug();
}
LightMapRenderer::~LightMapRenderer()
{
    delete m_PointLightMapArray;
    m_PointLightMapArray = NULL;
    delete m_DirectionalLightMapArray;
    m_DirectionalLightMapArray = NULL;
    delete m_SunLightMap;
    m_SunLightMap = NULL;
}

void LightMapRenderer::OnRenderLightMap(std::vector<Geometry*> geos, GLFWwindow *window)
{
    // ShadowCube render in one pass;
    OnRenderSingleLightMap(m_PointLightMapArray, geos, window);
    // CSM render in one pass;
    OnRenderSingleLightMap(m_DirectionalLightMapArray, geos, window);
}

void LightMapRenderer::OnRenderShadowSpace(GLFWwindow *window)
{
    if (settings::UISettings::GetSettings()->shadow) 
    {
        // this step does not drawcall, just generate space matrices and send to ubo.
        // real drawcall happends on shadowrenderpass.
        if (m_Lights->HasPointLights()) 
        {
            m_PointLightMapArray->Bind();
            m_PointLightMapArray->OnRenderLightSpace();
            m_PointLightMapArray->ActiveTexture();
        }

        if (m_Lights->HasDirectionalLights())
        {
            m_DirectionalLightMapArray->Bind();
            m_DirectionalLightMapArray->OnRenderLightSpace();
            m_DirectionalLightMapArray->ActiveTexture();
        }
        

        if (m_Lights->HasSunLight()) {
            m_SunLightMap->Bind();
            m_SunLightMap->OnRenderLightSpace();
            m_SunLightMap->ActiveTexture();
        }

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
}

void LightMapRenderer::OnRenderSingleLightMap(LightMapArray *lma, std::vector<Geometry*> &geos, GLFWwindow * window)
{

    lma->Bind();
    lma->OnRenderLightSpace();
    for (Geometry * g : geos)
    {
        lma->OnRenderGeometry(g, rc);
    }
    if (lma->IsDebugging())
    {
        for (Geometry *g : geos)
        {
            lma->RenderDebug(g);
        }
        throw JumpException("");
    }
    lma->UnBind();
}


void LightMapRenderer::SetSceneLights(LightCollection* l)
{
    m_Lights = l;
    m_PointLightMapArray->m_Lights = l;
    m_DirectionalLightMapArray->m_Lights = l;
}

void LightMapRenderer::ActiveTexture()
{
    m_PointLightMapArray->ActiveTexture();
    m_DirectionalLightMapArray->ActiveTexture();
}

void LightMapRenderer::RenderGeometry(Geometry *g, RenderContext &rc) const
{
    //lma->Bind();
// check empty
    if (m_Lights->HasDirectionalLights())
    {
        m_DirectionalLightMapArray->Bind();
        m_DirectionalLightMapArray->OnRenderGeometry(g, rc);
        m_DirectionalLightMapArray->UnBind();

        //m_DirectionalLightMapArray->EnableDebug();

        if (m_DirectionalLightMapArray->IsDebugging())
        {
            m_DirectionalLightMapArray->RenderDebug(g);
            throw JumpException("");
        }
    }

// check empty
    if (m_Lights->HasPointLights())
    {
        m_PointLightMapArray->Bind();
        m_PointLightMapArray->OnRenderGeometry(g, rc);
        m_PointLightMapArray->UnBind();

        //m_PointLightMapArray->EnableDebug();

        if (m_PointLightMapArray->IsDebugging())
        {
            m_PointLightMapArray->RenderDebug(g);
            throw JumpException("");
        }
    }


// check empty
    if (m_Lights->HasSunLight()) 
    {
        m_SunLightMap->Bind();
        m_SunLightMap->OnRenderGeometry(g, rc);
        m_SunLightMap->UnBind();

        //m_SunLightMap->EnableDebug();
        
        if (m_SunLightMap->IsDebugging())
        {
            m_SunLightMap->RenderDebug(g);
            throw JumpException("");
        }
    }
}

void LightMapRenderer::EnableSunLight(Light *light)
{
    m_SunLightMap->SetSunLight(static_cast<SunLight*>(light));
}