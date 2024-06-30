#include "ScreenCanvas.h"

const float screen_quad[] = {
    -1.0f,  1.0f,  0.0f, 1.0f,
    -1.0f, -1.0f,  0.0f, 0.0f,
    1.0f, -1.0f,  1.0f, 0.0f,

    -1.0f,  1.0f,  0.0f, 1.0f,
    1.0f, -1.0f,  1.0f, 0.0f,
    1.0f,  1.0f,  1.0f, 1.0f
};

const float screen_quad_window[] = {
    -1.0f,  1.0f,  0.0f, 1.0f,
    -1.0f, 0.0f,  0.0f, 0.0f,
    0.0f,  0.0f,  1.0f, 0.0f,

    -1.0f,  1.0f,  0.0f, 1.0f,
    0.0f, 0.0f,  1.0f, 0.0f,
    0.0f,  1.0f,  1.0f, 1.0f
};
ScreenCanvas::ScreenCanvas(bool fullScreen)
{
    InitVertices(fullScreen);
}

void ScreenCanvas::InitVertices(bool fullScreen)
{

    m_VB.Feed(fullScreen? screen_quad : screen_quad_window, 24 * sizeof(float));
    m_Layout.Push<float>(2);
    m_Layout.Push<float>(2);
    m_VAO.AddBuffer(m_VB, m_Layout);
}

void ScreenCanvas::Draw() const
{
   // glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glDisable(GL_DEPTH_TEST);
  //  glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
  //  glClear(GL_COLOR_BUFFER_BIT);
    m_VAO.Bind();
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_DEPTH_TEST);
}