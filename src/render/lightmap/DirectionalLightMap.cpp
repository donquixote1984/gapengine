#include "DirectionalLightMap.h"
#include "../../lights/DirectionalLight.h"

DirectionalLightMap::DirectionalLightMap(float width, float height):LightMap(width, height)
{
    glGenFramebuffers(1, &m_LightMapFrameBufferObj);
    glGenTextures(1, &m_LightMapTex);
    glBindTexture(GL_TEXTURE_2D, m_LightMapTex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, m_ScreenWidth, m_ScreenHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    GLfloat borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
    glBindFramebuffer(GL_FRAMEBUFFER, m_LightMapFrameBufferObj);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_LightMapTex, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);

    GLenum Status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (Status != GL_FRAMEBUFFER_COMPLETE) {
        printf("FB error, status: 0x%x\n", Status);
        throw std::exception();
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    InitShaders();
}

void DirectionalLightMap::InitShaders()
{
    m_LightMapShader      = new Shader("res/shaders/lightmap/directional/depth.shader");
    m_LightMapDebugShader = new Shader("res/shaders/lightmap/directional/debug.shader");

    ShaderSnippet::InstancingSnippet(5).ApplyToShader(m_LightMapShader);
}

void DirectionalLightMap::OnRenderLightSpace(Light *light)
{
    //m_LightMapShader->setUniform4m("lightSpaceMatrix", light->GetLightSpaceMat());
}
void DirectionalLightMap::OnRenderMVP(Geometry * g, Light * light)
{

    m_LightMapShader->Bind();
    m_LightMapShader->setUniform4m("model", g->GetModelMat());
    m_LightMapShader->setUniform4m("lightSpaceMatrix", GetLightSpaceMat(light));
    //m_LightMapShader->UnBind();
    if (g->IsInstancing())
    {
        m_LightMapShader->setUniform1i("instancing", 1);
    } else {
        m_LightMapShader->setUniform1i("instancing", 0);
    }
}


void DirectionalLightMap::RenderToGeometry(Geometry *g, Light *light)
{
    glActiveTexture(GL_TEXTURE31);
    glBindTexture(GL_TEXTURE_2D, m_LightMapTex);

    Shader * s = g->GetMaterial()->GetShader();
    s->setUniform1i("hasShadowMap", 1);
    s->setUniform1i("u_ShadowMap", constants::CSM);
    s->setUniform4m("lightSpaceMatrix", GetLightSpaceMat(light));

    
}

void DirectionalLightMap::RenderDebug(Geometry *g, Light * light)
{
    glViewport(0, 0, m_ScreenWidth, m_ScreenHeight);
    //glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glActiveTexture(GL_TEXTURE31);
    glBindTexture(GL_TEXTURE_2D, m_LightMapTex);
    m_LightMapDebugShader->Bind();
    m_LightMapDebugShader->setUniform1i("u_ShadowMap", constants::CSM);
    //m_LightMapDebugShader->setUniform1f("near_plane", light->GetNearPlane());
    //m_LightMapDebugShader->setUniform1f("far_plane", light->GetFarPlane());
    //g->RenderDrawCall();
    m_ScreenCanvas.Draw();
}

void DirectionalLightMap::Bind()
{
    glViewport(0, 0, m_ScreenWidth, m_ScreenHeight);
    glBindFramebuffer(GL_FRAMEBUFFER, m_LightMapFrameBufferObj);
    //glClearColor(0.1f, 0.1f, 0.3f, 1.0f);
    //glClearDepth(.5);
    glClear(GL_DEPTH_BUFFER_BIT);
}


glm::mat4 DirectionalLightMap::GetLightSpaceMat(Light * l)
{
    glm::vec3 origin = glm::vec3(0.0f) - glm::normalize(l->GetDirection()) * m_FarPlane / 2.0f;
    glm::vec3 dir = l->GetDirection();
    return m_LightOrthoMat * glm::lookAt(origin, glm::vec3(.0f), glm::vec3(0, 1, 0));
}