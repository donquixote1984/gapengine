#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "RawVertices.h"
#include "../VertexBuffer.h"
#include "../VertexBufferLayout.h"

class RawInstancingVertices: public RawVertices
{
protected:
    int m_InstanceCount = 0;
    glm::mat4 * m_InstancesMVP;    

    void DrawPoints() override;
    void DrawMesh() override;
    void DrawWireframe() override;

    VertexBuffer m_InstancingVB;
public:
    RawInstancingVertices();
    ~RawInstancingVertices();
    RawInstancingVertices(const RawInstancingVertices & r) = delete;
    void DrawInstance(DisplayMode mode);
    void SetInstancing(glm::mat4 * mvps, int instanceCount = 0);
    void SetInstancingMVP(glm::mat4 *mvps);
};