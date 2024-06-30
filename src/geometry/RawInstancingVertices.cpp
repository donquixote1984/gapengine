#include "RawInstancingVertices.h"

RawInstancingVertices::RawInstancingVertices()
{}
RawInstancingVertices::~RawInstancingVertices()
{}
void RawInstancingVertices::DrawInstance(DisplayMode mode)
{

}

void RawInstancingVertices::SetInstancing(glm::mat4 * mvps, int instanceCount)
{
    m_InstanceCount = instanceCount;
    SetInstancingMVP(mvps);
}

void RawInstancingVertices::SetInstancingMVP(glm::mat4 *mvps)
{
    m_InstancesMVP = mvps;
    m_InstancingVB.Bind();
    m_InstancingVB.Feed(m_InstancesMVP, m_InstanceCount * sizeof(glm::mat4));
    VertexBufferLayout layout;
    layout.Push<float>(4);
    layout.Push<float>(4);
    layout.Push<float>(4);
    layout.Push<float>(4);
    m_VAO.AddBuffer(m_InstancingVB, layout, 1);
}

void RawInstancingVertices::DrawPoints()
{
    glPointSize( 3.0 );
    glDrawArrays(GL_POINTS, 0, m_Count);
}
void RawInstancingVertices::DrawMesh()
{
    if (m_HasIndexBuffer)
    {
        m_EBO.Bind();
        //glDrawElements(GL_TRIANGLES, m_IndicesCount, GL_UNSIGNED_INT, NULL);
        glDrawElementsInstanced(GL_TRIANGLES, m_IndicesCount, GL_UNSIGNED_INT, 0, m_InstanceCount);
        m_EBO.Unbind();
    } else {
        glDrawArrays(GL_TRIANGLES, 0, m_Count);
    }
}
void RawInstancingVertices::DrawWireframe()
{

}
