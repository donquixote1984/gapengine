#pragma once
#include <string>
#include <glm/glm.hpp>
#include <unordered_map>
#include <any>
#include "./Shader.h"
enum class ShaderValueType
{
    INT,
    FLOAT,
    VEC2,
    VEC3,
    VEC4,
    MAT,
    BOOL,

    UNSUPPORTED,
};

struct ShaderValue
{
    ShaderValueType type = ShaderValueType::UNSUPPORTED;
    std::string key = "";
    std::any value;
};

class ShaderUniformsCache
{
private:
    std::unordered_map<std::string, ShaderValue> m_Caches;
public:
    void Cache(std::string key, float value);
    void Cache(std::string key, float v1, float v2);
    void Cache(std::string key, float v1, float v2, float v3);
    void Cache(std::string key, glm::vec3 v);
    void Cache(std::string key, glm::vec4 v);
    void Cache(std::string key, int v);
    void Cache(std::string key, unsigned int v);
    void Cache(std::string key, bool v);
    void Cache(std::string key, glm::mat4 m);
    void Clear();

    void FlushToShader(Shader* s);
    std::unordered_map<std::string, ShaderValue>& GetCaches();
};