#include <iostream>
#include <glad/glad.h>
#include "Renderer.h"


void glClearError()
{
	while(glGetError() != GL_NO_ERROR);

}

bool glLogCall()
{
	while(GLenum error = glGetError()) 
	{
		std::cout << "OpenGL Error: " << error << std::endl;
		return false;
	}
	return true;
}


void Renderer::Draw(const VertexArray & va, const IndexBuffer & ib,  Shader & shader)
{
	shader.Bind();
	va.Bind();
	ib.Bind();
	glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, 0);
}
void Renderer::Clear() const
{
	glClear(GL_COLOR_BUFFER_BIT);
}
