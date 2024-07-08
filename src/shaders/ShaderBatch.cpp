#include "ShaderBatch.h"

#define ASSERT(x) if (!(x)) assert(0);
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


ShaderBatch::ShaderBatch()
{
    InitConfig();
}

ShaderBatch::ShaderBatch(const std::string filePath) // cover Shader("")
{
    //Shader::Shader(filePath);
    m_Files.push_back(filePath);
}

ShaderBatch::ShaderBatch(std::initializer_list<std::filesystem::path> files)
{
    for(auto file: files)
    {
        m_Files.push_back(file);
    }
    InitConfig();
}

ShaderBatch::ShaderBatch(std::initializer_list<std::string> sources)
{
    for(auto source: sources)
    {
        m_Sources.push_back(source);
    }
    InitConfig();
}

void ShaderBatch::InitShader()
{
    if (m_HasCreated) {
		return;
	}
    unsigned int program = glCreateProgram();

    for (auto path: m_Files)
    {
        ShaderProgramSource sps = ParseShader(path.string(), SourceType::FILE);
        BatchCompile(sps, program);
    }

    for (auto source: m_Sources)
    {
        ShaderProgramSource sps = ParseShader(source, SourceType::SOURCE);
        BatchCompile(sps, program);
    }

    m_RendererID = program;
    GLCALL(glLinkProgram(program));
    int result;
    glGetProgramiv(m_RendererID, GL_LINK_STATUS, &result);
    if (result == GL_FALSE) 
	{
		int length; 
		glGetShaderiv(m_RendererID, GL_INFO_LOG_LENGTH, &length);
		char * message = (char*)alloca(200* sizeof(char));
		glGetProgramInfoLog(m_RendererID, 200, &length, message);
		std::cout << "failed to link shader: " << std::endl << message << std::endl;
		//glDeleteShader(id);
        assert(0);
	}
	m_HasCreated = true;
    
}

void ShaderBatch::BatchCompile(ShaderProgramSource &sps, unsigned int program)
{
   
    if (!sps.VertexSource.empty())
    {
        GLuint vs = CompileShader(GL_VERTEX_SHADER, sps.VertexSource);
        glAttachShader(program, vs);
    }

    if (!sps.FragmentSource.empty())
    {
        GLuint fs = CompileShader(GL_FRAGMENT_SHADER, sps.FragmentSource);
        glAttachShader(program, fs);
    }
    if (!sps.GeometrySource.empty())
    {
        GLuint gs = CompileShader(GL_GEOMETRY_SHADER, sps.GeometrySource);
        glAttachShader(program, gs);
    }

    if (!sps.TessellationControlSource.empty())
    {
        GLuint tsc = CompileShader(GL_TESS_CONTROL_SHADER, sps.TessellationControlSource);
        glAttachShader(program, tsc);
    }

    if (!sps.TessellationEvaluationSource.empty())
    {
        GLuint tse = CompileShader(GL_TESS_EVALUATION_SHADER, sps.TessellationEvaluationSource);
        glAttachShader(program, tse);
    }
}

void ShaderBatch::AddFile(const std::string filePath) // need copy 
{
    if (!filePath.empty())
    {
        m_Files.push_back(filePath);
    }
}
void ShaderBatch::AddSource(const std::string source) //  need copy
{
    if (!source.empty()) 
    {
        m_Sources.push_back(source);
    }
}