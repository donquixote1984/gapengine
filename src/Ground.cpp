#include "glad/glad.h"
#include "Ground.h"
#include <iostream>
#include <cstring>

Ground::Ground()
{
    InitGround();
}

Ground::~Ground()
{
    delete(m_Vertices);
    delete(m_Shader);
}

Ground::Ground(int rows):m_Rows(rows)
{
    InitGround();
}
Ground::Ground(int rows, float interval):m_Rows(rows), m_Interval(interval)
{
    InitGround();
}

Ground::Ground(int rows, float interval, glm::vec3 color): m_Rows(rows), m_Interval(interval), m_Color(color)
{
    InitGround();
}
void Ground::InitGround()
{
    GenVertices();
    GenIndices();
    GenBuffer();
    GenShader();
}

void Ground::GenShader()
{
    m_Shader = new Shader("./res/shaders/ground/ground.shader");
    m_Shader->Nohup();
}
void Ground::GenVertices()
{
    m_Vertices = new float[(m_Rows + 1) * (m_Rows + 1) * 3];
    float offset = m_Interval * m_Rows / 2;
    for (int i = 0; i < m_Rows + 1; i++)
    {
        for (int j = 0; j < m_Rows + 1; j++)
        {
            float x = j * m_Interval - offset;
            float y = 0;
            float z = i * m_Interval - offset;
            m_Vertices[i * (m_Rows + 1) * 3 + j*3] = x;
            m_Vertices[i * (m_Rows + 1) * 3+ j*3 + 1] = y;
            m_Vertices[i * (m_Rows + 1) * 3+ j*3 + 2] = z;
        }
    }
}

void Ground::GenIndices()
{
    m_Indices = new int[8 * m_Rows * m_Rows];
    for (int i = 0; i < m_Rows; i++)
    {
        for (int j = 0; j < m_Rows; j++)
        {
            float cell[8];
            cell[0] = i*(m_Rows+1) + j;
            cell[1] = i*(m_Rows+1) + j + 1;
            cell[2] = i*(m_Rows+1) + j + 1;
            cell[3] = (i+1)*(m_Rows+1) + j + 1;
            cell[4] = (i+1)*(m_Rows+1) + j + 1;
            cell[5] = (i+1)*(m_Rows+1) + j;
            cell[6] = (i+1)*(m_Rows+1) + j ;
            cell[7] = i*(m_Rows+1) + j;
            for (int k = 0; k < 8 ; k ++)
            {
                m_Indices[i * m_Rows * 8 + j * 8 + k] = cell[k];
            }
            //std::memcpy(m_Indices+ i * m_Rows * 8 + j * 8, cell, 8);
        }
    } 
}

int Ground::GetVerticesSize()
{
    return (m_Rows + 1) * (m_Rows + 1) * 3;
}

int Ground::GetIndicesSize()
{
    return 8 * m_Rows * m_Rows;
}
void Ground::GenBuffer()
{
    glGenVertexArrays(1, &m_VAO);
    glBindVertexArray(m_VAO);

    glGenBuffers(1, &m_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * GetVerticesSize(), m_Vertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);

    glGenBuffers(1, &m_EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(float) * GetIndicesSize(), m_Indices, GL_STATIC_DRAW);

    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

}

void Ground::Render()
{
    m_Shader->Bind();
     glBindVertexArray(m_VAO);
     glDrawElements(GL_LINES, 4 * GetIndicesSize(), GL_UNSIGNED_INT, NULL);
     glBindVertexArray(0);
}
void Ground::OnUpdateRender(RenderContext & rc)
{
    m_Shader->Bind();
    m_Shader->setUniform4m("projection", rc.projection);
    m_Shader->setUniform4m("view", rc.view);
    m_Shader->setUniform4m("model", glm::mat4(1));
    glBindVertexArray(m_VAO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
    glDrawElements(GL_LINES, 4 * GetIndicesSize(), GL_UNSIGNED_INT, NULL);
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}