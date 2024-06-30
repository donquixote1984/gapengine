#include "ShaderSnippet.h"

void ShaderSnippet::AddArgs(std::string key, std::string value)
{
    snippet[key] = value;
}
void ShaderSnippet::ApplyToShader(Shader * shader)
{
    for (auto &kv : snippet)
    {
        shader->AddArgs(kv.first, kv.second);
    }
}