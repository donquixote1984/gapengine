#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <initializer_list>
#include "../util/Util.h"
#include "../Vertex.h"
#include "../VertexBuffer.h"
#include "../VertexArray.h"
#include "../IndexBuffer.h"
#include "../VertexBufferLayout.h"
#include "DisplayMode.h"

class RawVertices
{
protected:
    float * m_Vertices = NULL;
    int * m_Indices = NULL;
    int m_Count;
    int m_IndicesCount;
    VertexBuffer m_VB;
    VertexArray m_VAO;
    IndexBuffer m_EBO;
    bool m_HasIndexBuffer = false;
    std::bitset<32> m_Layouts;

    bool m_Tessellation = false;
    int m_TessellationUnit = 4;
   
    RawVertices(const RawVertices & r) = delete;
    virtual void DrawPoints();
    virtual void DrawMesh();
    virtual void DrawWireframe();
public:
    RawVertices(); 
    ~RawVertices();
    RawVertices(float * vertices, int count, std::initializer_list<int> layouts);
    void Init(float * vertices, int count, std::initializer_list<int> layouts);
    void Init(float * vertices, int count);
    void Init(Vertex *vertices, int count);
    void InitIndex(int *indices, int indicesCount);
    void Buffer(std::bitset<32> layouts);
    void Buffer(std::initializer_list<int> layouts);
    virtual void Bind();
    virtual void UnBind();
    virtual void Draw();
    virtual void Draw(DisplayMode mode);
    void DrawPointCloud();
    void DumpToFile();
    void EnableTessellation(int unit);
};