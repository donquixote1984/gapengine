#include "LightMapArray.h"
#include <iostream>

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

#define ASSERT(x) if (!(x)) assert(0);

LightMapArray::LightMapArray(unsigned int textureType, unsigned int res, unsigned int size): m_Res(res), m_Size(size)
{
    glGenFramebuffers(1, &m_LightMapArrayFrameBufferObj);
    glGenTextures(1, &m_LightMapArrayTex);
    GLCALL(glBindTexture(textureType, m_LightMapArrayTex));
    glTexParameteri(textureType, GL_TEXTURE_BASE_LEVEL, 0);
    glTexParameteri(textureType, GL_TEXTURE_MAX_LEVEL, 0);
    glTexParameteri(textureType, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(textureType, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(textureType, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(textureType, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(textureType, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}

void LightMapArray::UnBind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, (GLsizei)m_ScreenWidth, (GLsizei)m_ScreenWidth);
}
LightMapArray::~LightMapArray()
{
    glDeleteFramebuffers(1, &m_LightMapArrayFrameBufferObj);
    glDeleteTextures(1, &m_LightMapArrayTex);
}

bool LightMapArray::IsDebugging()
{
    return m_IsDebugging;
}
void LightMapArray::EnableDebug()
{
    m_IsDebugging = true;
    m_LightMapDebugShader->Bind();
    m_LightMapDebugShader->UnBind();
}
void LightMapArray::OnRenderMVP(Geometry * geo, RenderContext &rc)
{
//    Shader * targetShader = geo->HasTess() ? m_LightMapTessShader : m_LightMapShader; (Mac M1 fallback on EVAL_PROG + GEOM_PROG)
    Shader *targetShader = m_LightMapShader;
    targetShader->Bind();
    targetShader->setUniform4m("model", geo->GetModelMat());
    if (geo->IsInstancing())
    {
        targetShader->setUniform1i("instancing", 1);
    } else {
        targetShader->setUniform1i("instancing", 0);
    }
    //animation goes here
    if (geo->GetGeoData()->HasAnimation() && geo->IsPlaying())
    {
        geo->PlayAnimation();
        targetShader->setUniform1i("u_IsPlaying", true);
        geo->GetAnimator()->UpdateAnimation(rc.deltaTime);
        std::vector<glm::mat4> transforms = geo->GetAnimator()->GetFinalBoneMatrices();
        for (int i = 0; i < transforms.size(); ++i)
        {
            targetShader->setUniform4m("finalBonesMatrices[" + std::to_string(i) + "]", transforms[i]);
        }
    }
    else
    {
        targetShader->setUniform1i("u_IsPlaying", false);
    }
}

void LightMapArray::OnRenderGeometry(Geometry * g, RenderContext &rc)
{
    
    //m_LightMapShader->Bind();
    //g->OnUpdateScript(window);
    if (HasLights()) {
        OnRenderMVP(g, rc);
        g->RenderDrawCall(m_LightMapShader);
    }
    
}