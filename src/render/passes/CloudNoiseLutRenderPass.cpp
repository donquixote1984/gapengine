#include "CloudNoiseLutRenderPass.h"
#include "../../global/ScreenProps.h"
CloudNoiseLutRenderPass::CloudNoiseLutRenderPass()
{
    m_FrameBuffer = new FrameBuffer(Global::screen.width, Global::screen.height, true);
    m_Shader = new Shader("res/shaders/volume/cloudnoiselut.shader");
    m_FrameBuffer->AddDrawShader(m_Shader);
}

CloudNoiseLutRenderPass::~CloudNoiseLutRenderPass()
{
    delete m_FrameBuffer;
    delete m_Shader;
}

void CloudNoiseLutRenderPass::Render(RenderContext &rc) 
{
    if (m_FirstRender) 
    {
        m_Shader->Bind();
        m_Shader->setUniform2f("u_Resolution", Global::screen.width, Global::screen.height);
        m_FrameBuffer->Render();
        m_FirstRender = false;
    } 
    m_FrameBuffer->ActiveTexture(0);
}


