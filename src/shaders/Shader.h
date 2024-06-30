#pragma once
#include <string>
#include <filesystem>
#include <map>
#include <mstch/mstch.hpp>
#include <glm/glm.hpp>
#include "../exceptions/ResourceNotFoundException.h"
#include "../Logger.h"
#include "ShaderTemplate.h"
#include "../UniformBuffer.h"
#include "ShaderType.h"
#include "../util/Util.h"
// settings
struct ShaderProgramSource
{
	std::string VertexSource;
	std::string FragmentSource;
    std::string GeometrySource;
    std::string TessellationControlSource;
    std::string TessellationEvaluationSource;
};

enum class SourceType
{
    FILE,
    SOURCE
};

class Shader
{
protected:
    std::string m_FilePath;
    std::string m_Source;
    unsigned int m_RendererID;
    //caching 
    std::map<std::string, int> m_LocationCache;
    bool m_HasCreated = false;
    std::map<std::string, std::string> m_Args = {};
    std::map<std::string, std::string> m_Partials = {};
    bool m_Nohup = false;
    void InitConfig();
    std::string m_CurrentFolder = "";
public:
    Shader();
    Shader(const std::string & filename);
    ~Shader();
    void Bind();
    void UnBind() const;
    void setUniform4f(const std::string & name, float v0, float v1, float v2, float v3);
    void setUniform1f(const std::string & name, float v0);
    void setUniform1i(const std::string & name, int v0);
    void setUniform2f(const std::string & name, float v0, float v1);
    void setUniform4m(const std::string &name, glm::mat4 proj);
    void setUniform4m(const std::string &name, float * arr);
    void setUniform3f(const std::string &name, float v0, float v1, float v2);
    void AddArgs(std::string key, std::string value);
    void AddArgs(std::map<std::string, std::string> map);
    void AddPartials(std::string name, std::string content);
    void Statistic();
    bool HasCompiled();
    void BindToUniformBuffer(UniformBuffer *ub, const char * channel);
    void BindToUniformBuffer(UniformBufferBindingSlot &ubs);
    void Ready();

    std::string ResolveInclude(const std::string& filepath, SourceType type = SourceType::FILE);
    std::string ResolveIncludeLine(const std::string &line, const std::string & currentPath, SourceType sourceType);
    unsigned int GetShaderId();
    virtual ShaderType GetShaderType();
    void SetCurrentFolder(const std::string &folder);
    void Nohup();
protected:
    int GetUniformLocation(const std::string & name);
    unsigned int CompileShader(unsigned int type, const std::string & source);
    unsigned int CreateShader(const std::string & vertexShader, const std::string & fragmentShader, const std::string & geometryShader, const std::string & tessellationControlShader,
	const std::string & tessellationEvaluationShader);
    ShaderProgramSource ParseShader(const std::string& filepath, SourceType type = SourceType::FILE);
    std::string ReplaceContent(std::string);
    virtual void InitShader();
    virtual void InitDefaultBindings();
    void OutputSrc();
    
};