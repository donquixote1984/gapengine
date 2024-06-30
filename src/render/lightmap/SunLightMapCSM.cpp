#include "SunLightMapCSM.h"
#include "../../global/GlobalData.h"

SunLightMapCSM::SunLightMapCSM(): LightMapArray(GL_TEXTURE_2D_ARRAY, Global::screen.width, 1), m_Camera(Global::camera)
{
    for (int i = 0; i < 4 ; i++)
    {
        m_ShadowCascadeLevels.push_back(m_Camera->GetCameraSettings().farPlane * lvls[i]);
    }
    glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_DEPTH_COMPONENT32F, m_ScreenWidth*2, m_ScreenWidth*2, (m_ShadowCascadeLevels.size() + 1) * m_Size, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
    constexpr float bordercolor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    glTexParameterfv(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_BORDER_COLOR, bordercolor);
    glBindFramebuffer(GL_FRAMEBUFFER, m_LightMapArrayFrameBufferObj);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, m_LightMapArrayTex, 0);

    GLenum Status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

    if (Status != GL_FRAMEBUFFER_COMPLETE) {
        printf("FB error, status: 0x%x\n", Status);
        throw std::exception();
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    m_UseCSM = true;
    InitShaders();
}

SunLightMapCSM::~SunLightMapCSM()
{}

void SunLightMapCSM::InitShaders()
{
    m_LightMapShader      = new Shader("res/shaders/lightmap/csm/sun.shader");
    m_LightMapDebugShader = new Shader("res/shaders/lightmap/csm/debug.shader");
    
    ShaderSnippet::InstancingSnippet(5).ApplyToShader(m_LightMapShader);
    m_LightMapShader->Ready();
    m_LightMapShader->BindToUniformBuffer(MatricesUniformBufferBindings::SUNSlot);
}

void SunLightMapCSM::OnRenderLightSpace()
{
    glClear(GL_DEPTH_BUFFER_BIT);
    m_LightMapShader->Bind();
    CalculateLightSpaceMatricesCSM(); // calculate camera based csm matrices and pass to ubo
}

void SunLightMapCSM::OnRenderGeometryCSM(Geometry *g)
{
    //glActiveTexture(GL_TEXTURE0 + constants::CSM);
    //glBindTexture(GL_TEXTURE_2D_ARRAY, m_LightMapTex);
    LightMapUtils::OnRenderGeometryCSM(g, m_Camera, m_ShadowCascadeLevels, true);
}
void SunLightMapCSM::OnRenderGeometry(Geometry *g)
{
    if (m_SunLight != nullptr) 
    {
        OnRenderGeometryCSM(g);
        LightMapArray::OnRenderGeometry(g);
    }
}

void SunLightMapCSM::ActiveTexture()
{
    glActiveTexture(GL_TEXTURE0 + constants::SUN_CSM);
    glBindTexture(GL_TEXTURE_2D_ARRAY, m_LightMapArrayTex);
}

void SunLightMapCSM::RenderDebug(Geometry *g)
{
    /*
    
    glViewport(0, 0, m_ScreenWidth*2, m_ScreenHeight*2);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClearColor(0,0,0,1);
    
    m_LightMapDebugShader->Bind();
    m_LightMapDebugShader->setUniform1i("layer", settings::UISettings::GetSettings()->csmlayer);

    glActiveTexture(GL_TEXTURE0 + constants::CSM);
    glBindTexture(GL_TEXTURE_2D_ARRAY, m_LightMapArrayTex);
    m_LightMapDebugShader->setUniform1i("u_CascadeShadowMap", constants::CSM);
   
    //m_LightMapDebugShader->setUniform1f("near_plane", l->GetNearPlane());
    //m_LightMapDebugShader->setUniform1f("far_plane", l->GetFarPlane());
    //g->RenderDrawCall();
    m_ScreenCanvas.Draw();
    m_LightMapDebugShader->UnBind();
    
    */
    glViewport(0, 0, m_ScreenWidth*2, m_ScreenHeight*2);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClearColor(0,0,0,1);
    
    m_LightMapDebugShader->Bind();
    m_LightMapDebugShader->setUniform1i("layer", settings::UISettings::GetSettings()->csmlayer);

    glActiveTexture(GL_TEXTURE0 + constants::SUN_CSM);
    glBindTexture(GL_TEXTURE_2D_ARRAY, m_LightMapArrayTex);
    m_LightMapDebugShader->setUniform1i("u_CascadeShadowMap", constants::SUN_CSM);
   
    //m_LightMapDebugShader->setUniform1f("near_plane", l->GetNearPlane());
    //m_LightMapDebugShader->setUniform1f("far_plane", l->GetFarPlane());
    m_ScreenCanvas.Draw();
    m_LightMapDebugShader->UnBind();
}
void SunLightMapCSM::Bind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, m_LightMapArrayFrameBufferObj);
    glViewport(0, 0, m_ScreenWidth * 2, m_ScreenWidth * 2);
}

void SunLightMapCSM::CalculateLightSpaceMatricesCSM()
{
    if (m_SunLight == nullptr)
    {
        return;
    }
    glm::mat4 shadowMapMatrices[constants::MAX_CSM_LVLS];
    shadowMapMatrices[0] = LightMapUtils::GetLightSpaceMatrix(m_Camera, m_SunLight, m_Camera->GetCameraSettings().nearPlane, m_ShadowCascadeLevels[0]);

    for (int i = 1; i < m_ShadowCascadeLevels.size(); i++)
    {
        shadowMapMatrices[i] = LightMapUtils::GetLightSpaceMatrix(m_Camera, m_SunLight, m_ShadowCascadeLevels[i - 1], m_ShadowCascadeLevels[i]);
        //GetLightSpaceMatrix(l);
    }
    shadowMapMatrices[constants::MAX_CSM_LVLS - 1] = LightMapUtils::GetLightSpaceMatrix(m_Camera, m_SunLight, m_ShadowCascadeLevels[constants::MAX_CSM_LVLS-2], m_Camera->GetCameraSettings().farPlane);
    //UniformBufferBindings::BindPSM
    MatricesUniformBufferBindings::FeedSUN(shadowMapMatrices);
}

bool SunLightMapCSM::HasLights()
{
    return m_SunLight != nullptr;
}

void SunLightMapCSM::SetSunLight(SunLight * light)
{
    m_SunLight = light;
}