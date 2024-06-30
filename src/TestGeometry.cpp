#include "TestGeometry.h"


static float skyboxVertices[] = {
    // positions          
    -1.0f,  1.0f, -1.0f,
    -1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

    -1.0f,  1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f,  1.0f
    };

TestGeometry::TestGeometry()
{
    ObjReader r ("/Users/guoxu.wu/dev/demo/ogl/opengl_assets/bin/temp/assets/models/grass/grass1/models/20240323201432001250.model.fbx");
    //data = new GeometryData(r.GetMeshes() ,std::byte{0b00101111});
    data= GeometryDataFactory::CreateGeometryData(r.GetMeshes());
       
      //  rv.Init(&(m_Meshes[0].vertices[0].position.x), m_Meshes[0].verticesCount);
      //  rv.InitIndex(m_Meshes[0].indices, m_Meshes[0].indicesCount);
      //  rv.Buffer(std::byte{0b00101111});
  //  std::vector<Mesh> m = r.GetMeshes();
    m_Shader = new Shader("res/shaders/test/geometry.geometry.shader");


 //   rv.Init(&(m[0].vertices[0].position.x), m[0].verticesCount);
 //   rv.InitIndex(m[0].indices, m[0].indicesCount);
}

void TestGeometry::OnRenderUpdate(RenderContext &rc)
{
    m_Shader->Bind();
    glm::mat4 model = glm::mat4(1);
    model = glm::scale(model, glm::vec3(.01, .01, .01));
    m_Shader->setUniform4m("model", model);
    //glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, NULL);
    //rv.Draw();
   //data->Draw();
   // glDrawArrays(GL_TRIANGLES, 0, 36);
}