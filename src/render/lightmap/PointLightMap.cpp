#include "PointLightMap.h"
#include "../../lights/PointLight.h"
#include "../../global/GlobalData.h"


PointLightMap::PointLightMap(float width, float height): LightMap(width, height)
{
    /*
    glGenFramebuffers(1, &m_LightMapFrameBufferObj);
    glGenTextures(1, &m_LightMapTex);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_LightMapTex);
    for (int i = 0; i < 6 ; i++)
    {
        // FATAL:   it is a cube,  the with and height must be equal.  
        // it is m_ScreenWidth, m_ScreenWidth,   rather than m_ScreenWidth ,  m_ScreenHeight
        
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, m_ScreenWidth, m_ScreenWidth, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    

    glBindFramebuffer(GL_FRAMEBUFFER, m_LightMapFrameBufferObj);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, m_LightMapTex, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    GLenum Status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

    if (Status != GL_FRAMEBUFFER_COMPLETE) {
        printf("FB error, status: 0x%x\n", Status);
        throw std::exception();
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    */
    InitShaders();
}

void PointLightMap::InitShaders()
{
    m_LightMapShader =      new Shader("res/shaders/lightmap/point/depth.shader");
    ShaderSnippet::InstancingSnippet(5).ApplyToShader(m_LightMapShader);
    m_LightMapDebugShader = new Shader("res/shaders/lightmap/point/debug.shader");
    //m_LightMapDebugShader = GeometryShaderFactory::CreateGeometryShader(ShaderType::DEFAULT_SHADER);
    GeometryShaderPartials::AddPartials(m_LightMapDebugShader);
    ShaderSnippet::ShadowSnippet().ApplyToShader(m_LightMapDebugShader);
    ShaderSnippet::InstancingSnippet(5).ApplyToShader(m_LightMapDebugShader);
    
}
void PointLightMap::OnRenderLightSpace(Light *light) 
{
    m_LightMapShader->Bind();
    /**
     *     
     * shadowTranforms.push_back(m_LightPerspectiveMat * glm::lookAt(GetPosition(), GetPosition() + glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
    shadowTranforms.push_back(m_LightPerspectiveMat * glm::lookAt(GetPosition(), GetPosition() + glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0, 0.0f)));

    shadowTranforms.push_back(m_LightPerspectiveMat * glm::lookAt(GetPosition(), GetPosition() + glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0, 1.0f)));
    shadowTranforms.push_back(m_LightPerspectiveMat * glm::lookAt(GetPosition(), GetPosition() + glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0, -1.0f)));

    shadowTranforms.push_back(m_LightPerspectiveMat * glm::lookAt(GetPosition(), GetPosition() + glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0, 0.0f)));
    shadowTranforms.push_back(m_LightPerspectiveMat * glm::lookAt(GetPosition(), GetPosition() + glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0, 0.0f)));
     *
    */
    //std::vector transforms = static_cast<PointLight*>(light)->GetShadowTransforms();
    //glm::mat4 perspective = light->GetPerspectiveMat();
    m_LightMapShader->setUniform4m("shadowMatrices[0]", m_LightPerspectiveMat * glm::lookAt(light->GetPosition(), light->GetPosition() + glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
    m_LightMapShader->setUniform4m("shadowMatrices[1]", m_LightPerspectiveMat * glm::lookAt(light->GetPosition(), light->GetPosition() + glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
    m_LightMapShader->setUniform4m("shadowMatrices[2]", m_LightPerspectiveMat * glm::lookAt(light->GetPosition(), light->GetPosition() + glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)));
    m_LightMapShader->setUniform4m("shadowMatrices[3]", m_LightPerspectiveMat * glm::lookAt(light->GetPosition(), light->GetPosition() + glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f)));
    m_LightMapShader->setUniform4m("shadowMatrices[4]", m_LightPerspectiveMat * glm::lookAt(light->GetPosition(), light->GetPosition() + glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
    m_LightMapShader->setUniform4m("shadowMatrices[5]", m_LightPerspectiveMat * glm::lookAt(light->GetPosition(), light->GetPosition() + glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
    m_LightMapShader->setUniform3f("lightPos", light->GetPosition().x, light->GetPosition().y, light->GetPosition().z);
    m_LightMapShader->setUniform1f("farPlane", m_FarPlane);
    m_LightMapShader->setUniform1i("instancing", 0);
}
void PointLightMap::OnRenderMVP(Geometry * geo, Light* light) 
{
    
    m_LightMapShader->Bind();
    m_LightMapShader->setUniform4m("model", geo->GetModelMat());
    if (geo->IsInstancing())
    {
        m_LightMapShader->setUniform1i("instancing", 1);
    } else {
        m_LightMapShader->setUniform1i("instancing", 0);
    }

    /**
     *     
     * shadowTranforms.push_back(m_LightPerspectiveMat * glm::lookAt(GetPosition(), GetPosition() + glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
    shadowTranforms.push_back(m_LightPerspectiveMat * glm::lookAt(GetPosition(), GetPosition() + glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0, 0.0f)));

    shadowTranforms.push_back(m_LightPerspectiveMat * glm::lookAt(GetPosition(), GetPosition() + glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0, 1.0f)));
    shadowTranforms.push_back(m_LightPerspectiveMat * glm::lookAt(GetPosition(), GetPosition() + glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0, -1.0f)));

    shadowTranforms.push_back(m_LightPerspectiveMat * glm::lookAt(GetPosition(), GetPosition() + glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0, 0.0f)));
    shadowTranforms.push_back(m_LightPerspectiveMat * glm::lookAt(GetPosition(), GetPosition() + glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0, 0.0f)));
     *
    */
    //m_LightMapShader->UnBind();

}

void PointLightMap::RenderToGeometry(Geometry *g, Light *l) // used at geometryrender
{
    //Global::scene->GetPointLightMapArray()->ActiveTexture();
    //Shader * s = g->GetMaterial()->GetShader();
    //s->setUniform1i("u_ShadowCubes", constants::SHADOWCUBE);
    g->GetUniforms().Cache("u_ShadowCubes", constants::SHADOWCUBE);


}

void PointLightMap::RenderDebug(Geometry*g, Light * light)
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glViewport(0, 0, m_ScreenWidth, m_ScreenHeight);
    //Global::scene->GetPointLightMapArray()->ActiveTexture();

    m_LightMapDebugShader->Bind();
    m_LightMapDebugShader->setUniform1i("u_ShadowCubes", constants::SHADOWCUBE);
    m_LightMapDebugShader->setUniform3f("u_PointLights[0].position", light->GetPosition().x, light->GetPosition().y, light->GetPosition().z);
    m_LightMapDebugShader->setUniform1f("u_PointLights[0].farPlane", m_FarPlane);

    m_LightMapDebugShader->setUniform4m("model", g->GetModelMat());
    if (g->IsInstancing())
    {
        m_LightMapDebugShader->setUniform1i("instancing", 1);
    } else {
        m_LightMapDebugShader->setUniform1i("instancing", 0);
    }
    g->RenderDrawCall();
    m_LightMapDebugShader->UnBind();
    //m_ScreenCanvas.Draw();
}

void PointLightMap::Bind()
{
    //glViewport(0, 0, m_ScreenWidth, m_ScreenWidth);
    //glBindFramebuffer(GL_FRAMEBUFFER, m_LightMapFrameBufferObj);
    //glClear(GL_DEPTH_BUFFER_BIT);
    //Global::scene->GetPointLightMapArray()->Bind(m_Index);
}