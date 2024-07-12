#include "EnvBox.h"
EnvBox::EnvBox(): RawVertices(util::UnitCube, 36, {3})
{
    m_Shader = new Shader("res/shaders/skybox/skybox.shader");
}
EnvBox::~EnvBox()
{
    delete m_Shader;
    m_Shader = nullptr;
}
void EnvBox::OnUpdateMVP(RenderContext &rc)
{
    if (m_HideBackground)
    {
        return;
    }
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    m_Shader->Bind();
    m_Shader->setUniform4m("projection", rc.projection);
    glm::mat4 view = glm::mat4(glm::mat3(rc.view));
    m_Shader->setUniform4m("view", view);
    BindTexture(1);
    m_Shader->setUniform1i("skybox", 1);
    m_VAO.Bind();
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
    glDepthFunc(GL_LESS);
}
void EnvBox::HideBackground(bool hide)
{
    m_HideBackground = hide;
}

void EnvBox::SetExposure(float exposure)
{
    m_Exposure = exposure;
}
float EnvBox::GetExposure()
{
    return m_Exposure;
}
