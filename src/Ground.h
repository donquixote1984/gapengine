#pragma once
#include "glm/glm.hpp"
#include "shaders/Shader.h"
#include "render/RenderContext.h"
class Ground
{
private:
    unsigned int m_VAO;
    unsigned int m_VBO;
    unsigned int m_EBO;
    int m_Rows=50;
    Shader *m_Shader;
    float m_Interval=0.2f;
    glm::vec3 m_Color=glm::vec3(1.0, 1.0, 1.0);
    void InitGround();
    void GenVertices();
    void GenIndices();
    void GenBuffer();
    int GetVerticesSize();
    int GetIndicesSize();
    void GenShader();
    void Output();
    float* m_Vertices;
    int* m_Indices;
public:
    Ground();
    ~Ground();
    Ground(int rows);
    Ground(int rows, float interval);
    Ground(int rows, float interval, glm::vec3 color);
    void Render();
    void OnUpdateRender(RenderContext &rc);
};