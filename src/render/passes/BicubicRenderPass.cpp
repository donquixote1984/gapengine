#include "BicubicRenderPass.h"
#include "../../global/GlobalData.h"

BicubicRenderPass::BicubicRenderPass(Bufferable *b)
{
    m_Bufferable = b;
    m_Shader = new Shader("res/shaders/volume/bicubic.shader");
}
BicubicRenderPass::~BicubicRenderPass()
{
    delete m_Shader;
    m_Shader = nullptr;
}
void BicubicRenderPass::Render(RenderContext &rc) 
{
    //m_Bufferable->Bind(0);
    m_Shader->Bind();
    m_Bufferable->ActiveTexture(0);
    m_Shader->setUniform1i("u_Texture", 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, Global::screen.width, Global::screen.height);
    m_ScreenCanvas.Draw();
}