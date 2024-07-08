#include "DirectionalLightMapArray.h"
#include "../../global/GlobalData.h"

#define GLCALL(x) glClearError();\
	x;\
	ASSERT(glLogCall())

static void glClearError()
{
	while(glGetError() != GL_NO_ERROR);

}
static bool glLogCall()
{
	while(GLenum error = glGetError()) 
	{
		std::cout << "OpenGL Error: " << error << std::endl;
		return false;
	}
	return true;
}

#define ASSERT(x) if (!(x)) __builtin_debugtrap();

DirectionalLightMapArray::DirectionalLightMapArray(): LightMapArray(GL_TEXTURE_2D_ARRAY, Global::screen.width, constants::MAX_DIR_LIGHTS), m_Camera(Global::camera)
{
    for (int i = 0; i < 4 ; i++)
    {
        m_ShadowCascadeLevels.push_back(m_Camera->GetCameraSettings().farPlane * lvls[i]);
    }
    glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_DEPTH_COMPONENT32F, (GLsizei)m_ScreenWidth*2, (GLsizei)m_ScreenWidth*2, (m_ShadowCascadeLevels.size() + 1) * m_Size, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
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
    InitShaders();
    EnableCSM(true);
}
DirectionalLightMapArray::~DirectionalLightMapArray()
{}

void DirectionalLightMapArray::Bind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, m_LightMapArrayFrameBufferObj);
    glViewport(0, 0, (GLsizei)m_ScreenWidth * 2, (GLsizei)m_ScreenWidth * 2);
    //glClear(GL_DEPTH_BUFFER_BIT);
}

void DirectionalLightMapArray::ActiveTexture()
{
    glActiveTexture(GL_TEXTURE0 + constants::CSM);
    glBindTexture(GL_TEXTURE_2D_ARRAY, m_LightMapArrayTex);
}

void DirectionalLightMapArray::OnRenderLightSpace()
{
    glClear(GL_DEPTH_BUFFER_BIT);
    m_LightMapShader->Bind();
    size_t dirLightCount = m_Lights->GetDirectionalLights().size();

    if (m_UseCSM) {
        CalculateLightSpaceMatricesCSM();
    } else {
        CalculateLightSpaceMatrices();
    }

    m_LightMapShader->setUniform1i("u_DirLightNum", dirLightCount);
    m_LightMapShader->setUniform1i("u_UseCSM", m_UseCSM);
    
}


void DirectionalLightMapArray::InitShaders()
{
    m_LightMapShader      = new Shader("res/shaders/lightmap/csm/depth.shader");
    ShaderSnippet::InstancingSnippet(7).ApplyToShader(m_LightMapShader);
    m_LightMapShader->Nohup();
    m_LightMapDebugShader = new Shader("res/shaders/lightmap/csm/debug.shader");
    m_LightMapDebugShader->Nohup();
    ShaderSnippet::AnimationSnippet().ApplyToShader(m_LightMapShader);
    m_LightMapShader->Ready();
    m_LightMapShader->BindToUniformBuffer(MatricesUniformBufferBindings::CSMSlot);
    m_LightMapShader->BindToUniformBuffer(BoneUniformBufferBindings::BoneSlot);
    GeometryShaderPartials::AddPartials(m_LightMapDebugShader);
    ShaderSnippet::InstancingSnippet(7).ApplyToShader(m_LightMapDebugShader);
}

void DirectionalLightMapArray::RenderDebug(Geometry*g)
{
    glViewport(0, 0, (GLsizei)m_ScreenWidth*2, (GLsizei)m_ScreenHeight*2);
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
}


void DirectionalLightMapArray::CalculateLightSpaceMatricesCSM()
{
    std::vector<DirectionalLight*> dirLights = m_Lights->GetDirectionalLights();

    //glm::mat4 shadowMapMatrices[dirLights.size() * constants::MAX_CSM_LVLS];
    glm::mat4* shadowMapMatrices = (glm::mat4*)_malloca(dirLights.size() * constants::MAX_CSM_LVLS * sizeof(glm::mat4));

    for (int i = 0; i < dirLights.size(); i++)
    {
        DirectionalLight *l = dirLights[i];
        shadowMapMatrices[i * constants::MAX_CSM_LVLS] = LightMapUtils::GetLightSpaceMatrix(m_Camera, l, m_Camera->GetCameraSettings().nearPlane, m_ShadowCascadeLevels[0]);
        int lvlCount = m_ShadowCascadeLevels.size();
        for ( int j = 1; j < lvlCount; j++)
        {
            shadowMapMatrices[i * constants::MAX_CSM_LVLS + j] = LightMapUtils::GetLightSpaceMatrix(m_Camera, l, m_ShadowCascadeLevels[j - 1], m_ShadowCascadeLevels[j]);
        }
        shadowMapMatrices[i * constants::MAX_CSM_LVLS + lvlCount] = LightMapUtils::GetLightSpaceMatrix(m_Camera, l, m_ShadowCascadeLevels[lvlCount - 1], m_Camera->GetCameraSettings().farPlane);
    }
    MatricesUniformBufferBindings::FeedCSM(shadowMapMatrices, dirLights.size());
}

void DirectionalLightMapArray::CalculateLightSpaceMatrices()
{
    std::vector<DirectionalLight*> dirLights = m_Lights->GetDirectionalLights();
    //glm::mat4 shadowMapMatrices[dirLights.size() * constants::MAX_CSM_LVLS];
    glm::mat4* shadowMapMatrices = (glm::mat4*)_malloca(dirLights.size() * constants::MAX_CSM_LVLS * sizeof(glm::mat4));
    
    for (int i = 0; i < dirLights.size(); i++)
    {
        DirectionalLight *l = dirLights[i];
        shadowMapMatrices[i * constants::MAX_CSM_LVLS] = LightMapUtils::GetLightSpaceMatrix(m_Camera, l, m_Camera->GetCameraSettings().nearPlane, m_Camera->GetCameraSettings().farPlane);
    }
    
   MatricesUniformBufferBindings::FeedCSM(shadowMapMatrices, dirLights.size());
}

void DirectionalLightMapArray::EnableCSM(bool csm)
{
    m_UseCSM = csm;
}

void DirectionalLightMapArray::OnRenderGeometryCSM(Geometry * g)
{
    LightMapUtils::OnRenderGeometryCSM(g, m_Camera, m_ShadowCascadeLevels, m_UseCSM);
}

void DirectionalLightMapArray::OnRenderGeometry(Geometry * g, RenderContext &rc)
{
    if (HasLights()) 
    {
        OnRenderGeometryCSM(g);
        LightMapArray::OnRenderGeometry(g, rc);
    }
}

bool DirectionalLightMapArray::HasLights() 
{
    if (m_Lights == nullptr)
    {
        return false;
    }
    return m_Lights->GetDirectionalLights().size() > 0;
}