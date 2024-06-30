#pragma once
#include <filesystem>
#include <initializer_list>
#include <vector>
#include "Shader.h"

class ShaderBatch: public Shader
{
private:
    std::vector<std::filesystem::path> m_Files;
    std::vector<std::string> m_Sources;
    void BatchCompile(ShaderProgramSource &sps, unsigned int program);
public:
    ShaderBatch();
    ShaderBatch(const std::string filePath);
    ShaderBatch(std::initializer_list<std::filesystem::path> files);
    ShaderBatch(std::initializer_list<std::string> sources);
    void InitShader() override;
    void AddFile(const std::string filePath);
    void AddSource(const std::string source);
};
