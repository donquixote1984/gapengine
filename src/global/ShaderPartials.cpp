#include "ShaderPartials.h"

namespace Global
{
    std::unordered_map<std::string, std::string> ShaderPartialsPool;
    std::string ShaderPartialsPrefixPath = "res/shaders/partials";
}


 std::string Global::GetShaderPartials(const std::string &key)
 {
    if (ShaderPartialsPool.find(key) == ShaderPartialsPool.end())
    {
        std::string pathkey = ShaderPartialsPrefixPath + "/" + key;
        if (!std::filesystem::exists(pathkey))
        {
            throw ResourceNotFoundException(pathkey.c_str());
        }
        std::string content = "\n" + util::ReadFile(pathkey);
        ShaderPartialsPool[key] = content;
        return content;
    }

    return ShaderPartialsPool[key];
 }