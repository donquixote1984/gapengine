#pragma once
#include <string>
#include <unordered_map>
#include "../util/Util.h"
#include "../exceptions/ResourceNotFoundException.h"

namespace Global
{
    extern std::unordered_map<std::string, std::string> ShaderPartialsPool;
    extern std::string ShaderPartialsPrefixPath;
    std::string GetShaderPartials(const std::string &key);
};