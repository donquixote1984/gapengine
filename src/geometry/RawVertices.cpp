#include "RawVertices.h"

#define GLCALL(x) glClearError();\
	x;\
	ASSERT(glLogCall())

static void glClearError()
{
	while(glGetError() != GL_NO_ERROR);

}
static bool glLogCall()
{
	while(GLenum error = glGetError()) 
	{
		std::cout << "OpenGL Error: " << error << std::endl;
		return false;
	}
	return true;
}

#define ASSERT(x) if (!(x)) __builtin_debugtrap();

RawVertices::RawVertices()
{}
RawVertices::~RawVertices()
{
    //delete m_Vertices;
    //m_Vertices = NULL;
}
RawVertices::RawVertices(float * vertices, int count, std::initializer_list<int> layouts): m_Vertices(vertices), m_Count(count)
{
    Buffer(util::ILToByte(layouts));
}
void RawVertices::Buffer(std::initializer_list<int> layouts)
{
    std::bitset<32> bLayouts = util::ILToByte(layouts);
    Buffer(bLayouts);
}
void RawVertices::Buffer(std::bitset<32> layouts)
{
    
    m_Layouts = layouts;
    VertexBufferLayout vlayout;
    int stride = 0;
    int size = util::GetLayoutSize(layouts);
    for (int i = 0; i < size ; i++)
    {
        int d = (layouts & std::bitset<32>{"0000000000000111"}).to_ulong();
        stride += d;
        layouts >>= 3;
        if (d != 0 ) 
        {
            vlayout.Push<float>(d);
        }
    }
 
    m_VAO.Bind();
    m_VB.Bind();
    m_VB.Feed(m_Vertices, stride * m_Count* sizeof(float));

    if (m_HasIndexBuffer)
    {
        m_EBO.Bind();
        m_EBO.Feed(m_Indices, m_IndicesCount);
       
    }
    m_VAO.AddBuffer(m_VB, vlayout);
    m_VB.Unbind();
    m_VAO.UnBind();
}
void RawVertices::Init(float * vertices, int count, std::initializer_list<int> layouts)
{
    m_Vertices = vertices;
    m_Count= count;

    Buffer(util::ILToByte(layouts));
}

void RawVertices::Init(float * vertices, int count)
{
    m_Vertices = vertices;
    m_Count= count;
  
    //Buffer(layouts);
}

void RawVertices::InitIndex(int *indices, int indicesCount)
{
    m_HasIndexBuffer = true;
    m_IndicesCount = indicesCount;
    m_Indices = indices;
}

void RawVertices::Init(Vertex *vertex, int count)
{
    Init(&vertex->position.x, count);
}

void RawVertices::Bind()
{
    m_VAO.Bind();
    
}

void RawVertices::UnBind()
{
    m_VAO.UnBind();
}

void RawVertices::Draw()
{
    m_VAO.Bind();
    if (m_HasIndexBuffer)
    {
        m_EBO.Bind();
        glDrawElements(GL_TRIANGLES, m_IndicesCount, GL_UNSIGNED_INT, NULL);
        m_EBO.Unbind();
    } else {
        glDrawArrays(GL_TRIANGLES, 0, m_Count);
    }
    m_VAO.UnBind();
}

void RawVertices::Draw(DisplayMode mode)
{
    m_VAO.Bind();
    switch(mode)
    {
        case DisplayMode::POINTS:
            DrawPoints();
            break;
        case DisplayMode::MESH:
            DrawMesh();
            break;
        case DisplayMode::WIREFRAME:
            DrawWireframe();
            break;
        default:
            DrawMesh();
    }
    m_VAO.UnBind();
}

void RawVertices::DrawPoints()
{
    glPointSize( 3.0 );
    glDrawArrays(GL_POINTS, 0, m_Count);
}

void RawVertices::DrawMesh()
{
    if (m_Tessellation)
    {
        glDrawArrays(GL_PATCHES, 0, m_Count);
        return;
    }
    if (m_HasIndexBuffer)
    {
        m_EBO.Bind();
        glDrawElements(GL_TRIANGLES, m_IndicesCount, GL_UNSIGNED_INT, NULL);
        m_EBO.Unbind();
        return;
    } 
    glDrawArrays(GL_TRIANGLES, 0, m_Count);
}

void RawVertices::DrawWireframe()
{
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    if (m_Tessellation)
    {
        glDrawArrays(GL_PATCHES, 0, m_Count);
        return;
    }
    //glLineWidth(1.0f);
    if (m_HasIndexBuffer)
    {
        m_EBO.Bind();
        glDrawElements(GL_TRIANGLES, m_IndicesCount, GL_UNSIGNED_INT, NULL);
        m_EBO.Unbind();
        return;
    } 
    glDrawArrays(GL_TRIANGLES, 0, m_Count);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    //glPolygonMode(GL_BACK, GL_FILL);

}



void RawVertices::DrawPointCloud()
{
    m_VAO.Bind();
    glDrawArrays(GL_POINTS, 0, m_Count);
    m_VAO.UnBind();
}

void RawVertices::EnableTessellation(int unit)
{
    m_Tessellation = true;
    m_TessellationUnit = unit;
}

void RawVertices::DumpToFile()
{
    std::ofstream outfile;
    outfile.open("dump.txt", std::ios::out);
    std::string verticesStr;
    std::stringstream ss;
    ss << "Layouts: " << m_Layouts << std::endl;
    ss << "Vertices: " << std::endl;
    for (int i = 0 ;i < m_Count; i++)
    {
        ss << std::to_string(m_Vertices[i]);
        ss << ", ";
    }
    ss << std::endl;
    ss << "Indices: ";
    ss << std::endl;
    if (m_HasIndexBuffer)
    {
        for (int i = 0; i < m_IndicesCount ;i ++)
        {
            ss << m_Indices[i] << std::endl;
            ss << ", ";
        }
    }

    outfile << ss.str();
    outfile.flush();
    outfile.close();

}

