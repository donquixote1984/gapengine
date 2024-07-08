#include "PointLightMapArray.h"
PointLightMapArray::PointLightMapArray(): LightMapArray(GL_TEXTURE_CUBE_MAP_ARRAY, Global::screen.width, constants::MAX_POINT_LIGHTS)
{
    //GLCALL(glTexImage3D(textureType, 0, GL_DEPTH_COMPONENT, m_Res, m_Res, 6 * size, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr));
    glTexImage3D(GL_TEXTURE_CUBE_MAP_ARRAY, 0, GL_DEPTH_COMPONENT, m_Res, m_Res, 6 * m_Size, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
    glBindFramebuffer(GL_FRAMEBUFFER, m_LightMapArrayFrameBufferObj);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, m_LightMapArrayTex, 0);

    GLenum Status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

    if (Status != GL_FRAMEBUFFER_COMPLETE) {
        printf("FB error, status: 0x%x\n", Status);
        throw std::exception();
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    InitShaders();
}
PointLightMapArray::~PointLightMapArray()
{}
void PointLightMapArray::Bind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, m_LightMapArrayFrameBufferObj);
   // glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, m_LightMapArrayTex, 0);
    glViewport(0, 0, Global::screen.width, Global::screen.width);
    //glClear(GL_DEPTH_BUFFER_BIT);
}

void PointLightMapArray::ActiveTexture()
{
    glActiveTexture(GL_TEXTURE0 + constants::SHADOWCUBE);
    glBindTexture(GL_TEXTURE_CUBE_MAP_ARRAY, m_LightMapArrayTex);
}

void PointLightMapArray::OnRenderLightSpace()
{
    glClear(GL_DEPTH_BUFFER_BIT);
    m_LightMapShader->Bind();
    int pointLightCount = m_Lights->GetPointLights().size();
    
    //glm::mat4 shadowCubeMatrices[pointLightCount * 6];
    glm::mat4* shadowCubeMatrices = (glm::mat4*)_malloca(pointLightCount * 6 * sizeof(glm::mat4));

    for (int i = 0; i < pointLightCount; i++)
    {
        PointLight *pointLight = m_Lights->GetPointLights()[i];
        for (int face = 0; face < 6; face++)
        {
            shadowCubeMatrices[i * 6 + face] = m_LightPerspectiveMat * glm::lookAt(pointLight->GetPosition(), pointLight->GetPosition() + m_LookAts[face].first, m_LookAts[face].second);
        }
       
        m_LightMapShader->setUniform1i("u_PointLights["+std::to_string(pointLight->GetIndex())+"].index", pointLight->GetIndex());
        m_LightMapShader->setUniform3f("u_PointLights["+std::to_string(pointLight->GetIndex())+"].position", pointLight->GetPosition().x, pointLight->GetPosition().y, pointLight->GetPosition().z);
        m_LightMapShader->setUniform1f("u_PointLights["+std::to_string(pointLight->GetIndex())+"].farPlane", pointLight->GetFarPlane());

        //OnRenderSingleSpace(pointLight);
    }
    //m_LightMapShader->setUniform1f("farPlane", pointLight);
    m_LightMapShader->setUniform1i("instancing", 0);
    m_LightMapShader->setUniform1i("u_PointLightNum", pointLightCount);

    MatricesUniformBufferBindings::FeedPSM(shadowCubeMatrices, pointLightCount);
}

void PointLightMapArray::InitShaders()
{
    m_LightMapShader =      new Shader("res/shaders/lightmap/point/depth.shader");
    m_LightMapShader->Nohup();
    ShaderSnippet::AnimationSnippet().ApplyToShader(m_LightMapShader);
    ShaderSnippet::InstancingSnippet(7).ApplyToShader(m_LightMapShader);

    m_LightMapDebugShader = new Shader("res/shaders/lightmap/point/debug.shader");
    m_LightMapDebugShader->Nohup();

    //m_LightMapDebugShader = GeometryShaderFactory::CreateGeometryShader(ShaderType::DEFAULT_SHADER);
    GeometryShaderPartials::AddPartials(m_LightMapDebugShader);
    ShaderSnippet::ShadowSnippet().ApplyToShader(m_LightMapDebugShader);
    ShaderSnippet::InstancingSnippet(5).ApplyToShader(m_LightMapDebugShader);
    m_LightMapShader->Ready();
    m_LightMapShader->BindToUniformBuffer(MatricesUniformBufferBindings::PSMSlot);
    m_LightMapShader->BindToUniformBuffer(BoneUniformBufferBindings::BoneSlot);
}

void PointLightMapArray::RenderDebug(Geometry*g)
{
    Light * light = m_Lights->GetLights()[1];
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glActiveTexture(GL_TEXTURE0 + constants::SHADOWCUBE);
    glBindTexture(GL_TEXTURE_CUBE_MAP_ARRAY, m_LightMapArrayTex);
    glClearColor(.5f, .5f, .5f, .5f);
    glViewport(0, 0, m_ScreenWidth, m_ScreenHeight);
    //Global::scene->GetPointLightMapArray()->ActiveTexture();
    m_LightMapDebugShader->Bind();
    m_LightMapDebugShader->setUniform1i("u_ShadowCubes", constants::SHADOWCUBE);
    m_LightMapDebugShader->setUniform3f("u_PointLights[1].position", light->GetPosition().x, light->GetPosition().y, light->GetPosition().z);
    m_LightMapDebugShader->setUniform1f("u_PointLights[1].farPlane", light->GetFarPlane());
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
bool PointLightMapArray::HasLights() 
{
    if (m_Lights == nullptr)
    {
        return false;
    }
    return m_Lights->GetPointLights().size() > 0;
}