#include "RawVerticesTessellation.h"

RawVerticesTessellation::RawVerticesTessellation(float * vertices, int count, std::initializer_list<int> layouts): RawVertices(vertices, count, layouts)
{
    glPatchParameteri(GL_PATCH_VERTICES, m_Unit);
}
void RawVerticesTessellation::Draw()
{
    m_VAO.Bind();
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glPatchParameteri(GL_PATCH_VERTICES, m_Unit);
    glDrawArrays(GL_PATCHES, 0, m_Count);
    m_VAO.UnBind();
}

void RawVerticesTessellation::Draw(DisplayMode mode)
{
    m_VAO.Bind();
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glPatchParameteri(GL_PATCH_VERTICES, m_Unit);
    glDrawArrays(GL_PATCHES, 0, m_Count);
    m_VAO.UnBind();
}