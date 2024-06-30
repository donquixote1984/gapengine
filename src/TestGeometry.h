#pragma once
#include <glad/glad.h>
#include "reader/ObjReader.h"
#include "VertexBuffer.h"
#include "VertexArray.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"
#include "geometry/RawVertices.h"
#include "geometry/GeometryData.h"
#include "render/RenderContext.h"

class TestGeometry
{
private:
//    VertexBuffer m_VBO;
//    VertexArray m_VAO;
//    IndexBuffer m_EBO;
    Shader *m_Shader;
    GeometryData *data;
    RawVertices rv;
public:
    TestGeometry();
    void OnRenderUpdate(RenderContext &rc);
};