#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "shaders/Shader.h"
#include "IndexBuffer.h"
#include "VertexArray.h"

#define ASSERT(x) if (!(x)) __builtin_debugtrap();
#define GLCALL(x) glClearError();\
	x;\
	ASSERT(glLogCall())


// settings
const unsigned int SCR_WIDTH = 1200;
const unsigned int SCR_HEIGHT = 1080;

void glClearError();
bool glLogCall();

class Renderer
{
public:
	void Draw(const VertexArray & vertexArray, const IndexBuffer & ib,  Shader & shader) ;
	void Clear() const;
};