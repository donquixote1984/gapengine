#include <glad/glad.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <boost/algorithm/string.hpp>
#include <filesystem>
#include "Shader.h"

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


Shader::Shader()
{}
void Shader::InitConfig()
{
	m_Args = ShaderTemplateKeys::GetDefault();
	mstch::config::escape = [](const std::string& str) -> std::string {
		return str;
	};
}
Shader::Shader(const std::string & filename) : m_FilePath(filename), m_RendererID(0)
{
	InitConfig();
}

Shader::~Shader() 
{
    glDeleteProgram(m_RendererID);
}


void Shader::Bind()
{
	if (!m_HasCreated) {
		Ready();
	}
    glUseProgram(m_RendererID);
}

void Shader::UnBind() const
{
    glUseProgram(0);
}

void Shader::setUniform4f(const std::string & name, float v0, float v1, float v2, float v3)
{
    int location = GetUniformLocation(name);
    glUniform4f(location, v0, v1, v2, v3);
}

void Shader::setUniform3f(const std::string &name, float v0, float v1, float v2)
{
	 int location = GetUniformLocation(name);
    glUniform3f(location, v0, v1, v2);
}
void Shader::setUniform2f(const std::string &name, float v0, float v1)
{
	int location = GetUniformLocation(name);
	glUniform2f(location, v0, v1);
}

void Shader::setUniform1f(const std::string &name, float v0)
{
    int location = GetUniformLocation(name);
    glUniform1fv(location, 1, &v0);
}

void Shader::setUniform1i(const std::string &name, int v0)
{
    int location = GetUniformLocation(name);
    glUniform1i(location, v0);
}
void Shader::setUniform4m(const std::string &name, glm::mat4 proj)
{
    int location = GetUniformLocation(name);
    glUniformMatrix4fv(location, 1, GL_FALSE, &proj[0][0]);
}
void Shader::setUniform4m(const std::string &name, float * arr)
{
    int location = GetUniformLocation(name);
    glUniformMatrix4fv(location, 1, GL_FALSE, arr);
}

int Shader::GetUniformLocation(const std::string & name)
{
    if (m_LocationCache.find(name) != m_LocationCache.end())
    {
        return m_LocationCache[name];
    }
    int location = glGetUniformLocation(m_RendererID, name.c_str());
    if (location == -1)
    {
        std::cout << "Warning: uniform '"<< name <<"' does not exist in: " <<m_FilePath<< std::endl;
    }
    else
    {
        m_LocationCache[name] = location;
    }

    return location;
}

std::string Shader::ResolveIncludeLine(const std::string &line, const std::string & currentPath, SourceType sourceType)
{
	std::string replaceline = ReplaceContent(line);
	if (replaceline.find("#include") != std::string::npos)
	{
		boost::replace_all(replaceline, "#include", "");
		boost::trim(replaceline);
		replaceline = replaceline.substr(1, replaceline.length()-2);
		std::filesystem::path path = currentPath;
		const std::string &includepath = path.parent_path().append(replaceline).string();
		if (std::filesystem::exists(includepath)) 
		{
			std::string includecontent = ResolveInclude(includepath, SourceType::FILE);
			return includecontent;
		} else {
			return "";
		} 
	} else {
		return replaceline;
	}
}
std::string Shader::ResolveInclude(const std::string& filepath, SourceType sourceType)
{
	std::string line;
	std::stringstream includestream;

	if (sourceType == SourceType::FILE) {
		std::fstream stream(filepath);
		while (getline(stream, line))
		{
			std::string resolvedLine = ResolveIncludeLine(line, filepath, sourceType);
			includestream << resolvedLine << "\n";
		}
		stream.close();
	} else {
		std::stringstream stream;
		stream << filepath;
		while (getline(stream, line))
		{
			std::string resolvedLine = ResolveIncludeLine(line, m_CurrentFolder, sourceType);
			includestream << resolvedLine << "\n";
		}
	}
	
	return includestream.str();
}

ShaderProgramSource Shader::ParseShader(const std::string& filepath, SourceType sourceType)
{
	if (sourceType == SourceType::FILE) {
	if (!std::filesystem::exists(filepath))
	{
		throw ResourceNotFoundException(filepath.c_str());
	}
	}

	//std::fstream stream(filepath);
	std::string line;
	std::stringstream sourcestream;
	std::stringstream ss[5];
	enum class ShaderType 
	{
		NONE = -1, VERTEX = 0 , FRAGMENT = 1, GEOMETRY = 2,
		TESS_C = 3,
		TESS_E = 4
	};
	mstch::map ctx;
	for ( auto & kv : m_Args)
	{
		ctx[kv.first] = kv.second;
	}

	ShaderType type = ShaderType::NONE;
	std::string source = ResolveInclude(filepath, sourceType);
	sourcestream << source ;

	while (getline(sourcestream, line))
	{
		if (line.find("#shader") != std::string::npos)
		{
			if (line.find("vertex") != std::string::npos)
			{
				type = ShaderType::VERTEX;
			}
			else if (line.find("fragment") != std::string::npos)
			{
				type = ShaderType::FRAGMENT;
			}
			else if (line.find("geometry") != std::string::npos)
			{
				type = ShaderType::GEOMETRY;
			}
			else if (line.find("tessellation control") != std::string::npos)
			{
				type = ShaderType::TESS_C;
			}
			else if (line.find("tessellation evaluation") != std::string::npos)
			{
				type = ShaderType::TESS_E;
			}

		}
		else
		{
			if (type != ShaderType::NONE) {
				ss[(int)type] << line << "\n";
			}
		}
	}
	return {ss[0].str(), ss[1].str(), ss[2].str(), ss[3].str(), ss[4].str()};
}
unsigned int Shader::CompileShader(unsigned int type, const std::string & source) 
{
	unsigned int id = glCreateShader(type);
	std::string realSrc = ReplaceContent(source);
	m_Source += realSrc;
	const char * src = realSrc.c_str();
	glShaderSource(id, 1, &src, nullptr);
	glCompileShader(id);
	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if (!this->m_Nohup) {
		Logger::GetLogger()->log("[shader source]" + realSrc);
	}
	if (result == GL_FALSE) 
	{
		int length; 
		Logger::GetLogger()->log("[shader compile failure]" + realSrc);
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		char * message = (char*)alloca(length * sizeof(char));
		glGetShaderInfoLog(id, length, &length, message);
		std::cout << "failed to compile shader: " << std::endl << message << std::endl;
		//glDeleteShader(id);
		
		assert(0);
		return 0;
	}

	return id;
}
unsigned int Shader::CreateShader(
	const std::string & vertexShader, 
	const std::string & fragmentShader, 
	const std::string & geometryShader,
	const std::string & tessellationControlShader,
	const std::string & tessellationEvaluationShader
	) 
{
	GLuint program = glCreateProgram();
	GLuint vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
	GLuint fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);
	glAttachShader(program, vs);
	glAttachShader(program, fs);
	if (!geometryShader.empty())
	{
		GLuint gs = CompileShader(GL_GEOMETRY_SHADER, geometryShader);
		glAttachShader(program, gs);
	}

	if (!tessellationControlShader.empty())
	{
		GLuint tsc = CompileShader(GL_TESS_CONTROL_SHADER, tessellationControlShader);
		glAttachShader(program, tsc);
	}

	if (!tessellationEvaluationShader.empty())
	{
		GLuint tse = CompileShader(GL_TESS_EVALUATION_SHADER, tessellationEvaluationShader);
		glAttachShader(program, tse);
	}
	GLCALL(glLinkProgram(program));

	int result;
	glGetProgramiv(program, GL_LINK_STATUS, &result);
	if (result == GL_FALSE)
	{
		int length;
		glGetShaderiv(program, GL_INFO_LOG_LENGTH, &length);
		char* message = (char*)alloca(200 * sizeof(char));
		glGetProgramInfoLog(program, 200, &length, message);
		std::cout << "failed to link shader: " << std::endl << message << std::endl;
		//glDeleteShader(id);
		assert(0);
	}


	return program;
}

void Shader::AddArgs(std::string key, std::string value)
{
	m_Args[key] = value;
}
void Shader::AddArgs(std::map<std::string, std::string> map)
{
	for (auto & d : map)
	{
		m_Args[d.first] = d.second;
	}
}
void Shader::InitShader()
{
	if (m_HasCreated) {
		return;
	}
	ShaderProgramSource sps = ParseShader(m_FilePath);
	m_RendererID = CreateShader(sps.VertexSource, sps.FragmentSource, sps.GeometrySource, sps.TessellationControlSource, sps.TessellationEvaluationSource);
	m_HasCreated = true;
}

void Shader::AddPartials(std::string name, std::string content)
{
	m_Partials[name] = content;
}

std::string Shader::ReplaceContent(std::string code)
{
	mstch::map ctx;
	for ( auto & kv : m_Args)
	{
		ctx[kv.first] = kv.second;
	}

	return mstch::render(code, ctx, m_Partials);
}
void Shader::Statistic()
{
	GLint i;
	GLint count;

	GLint size; // size of the variable
	GLenum type; // type of the variable (float, vec3 or mat4, etc)

	const GLsizei bufSize = 32; // maximum name length
	GLchar name[bufSize]; // variable name in GLSL
	GLsizei length; // name length
	glGetProgramiv(m_RendererID, GL_ACTIVE_ATTRIBUTES, &count);
	printf("Active Attributes: %d\n", count);

	for (i = 0; i < count; i++)
	{
		glGetActiveAttrib(m_RendererID, (GLuint)i, bufSize, &length, &size, &type, name);

		printf("Attribute #%d Type: %u Name: %s\n", i, type, name);
	}

	glGetProgramiv(m_RendererID, GL_ACTIVE_UNIFORMS, &count);
	printf("Active Uniforms: %d\n", count);

	for (i = 0; i < count; i++)
	{
		glGetActiveUniform(m_RendererID, (GLuint)i, bufSize, &length, &size, &type, name);

		printf("Uniform #%d Type: %u Name: %s\n", i, type, name);
	}
}

bool Shader::HasCompiled()
{
	return m_HasCreated;
}

void Shader::InitDefaultBindings() {}

void Shader::BindToUniformBuffer(UniformBuffer *ub, const char * channel)
{
	//ub->BindShaderSlot(m_RendererID, channel);
	//UniformBufferBindings::Bind
}
void Shader::BindToUniformBuffer(UniformBufferBindingSlot &ubs)
{
	UniformBufferBindings::BindSlot(m_RendererID, ubs);
}
//void Shader::BindToUniformBuffer(UniformBUfferBindings bindings, Uniform)
void Shader::Ready()
{
	if (m_HasCreated)
	{
		return;
	}
	InitShader();
	InitDefaultBindings();
}

 unsigned int Shader::GetShaderId()
 {
	return m_RendererID;
 }

ShaderType Shader::GetShaderType()
{
	return ShaderType::RAW;
}
void Shader::OutputSrc()
{
	std::cout << m_Source << std::endl;
}
void Shader::Nohup()
{
	m_Nohup = true;
}

void Shader::SetCurrentFolder(const std::string &folder)
{
	m_CurrentFolder = folder;
}