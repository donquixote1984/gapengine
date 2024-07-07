#include "ShaderUniformsCache.h"

void ShaderUniformsCache::Cache(std::string key, float value)
{
    ShaderValue v = { ShaderValueType::FLOAT, key, value };
    m_Caches[key] = v;
}
void ShaderUniformsCache::Cache(std::string key, float v1, float v2)
{
    ShaderValue v = { ShaderValueType::VEC2, key, glm::vec2(v1, v2) };
    m_Caches[key] = v;
}
void ShaderUniformsCache::Cache(std::string key, float v1, float v2, float v3)
{
    ShaderValue v = { ShaderValueType::VEC3, key, glm::vec3(v1,v2,v3) };
    m_Caches[key] = v;
}
void ShaderUniformsCache::Cache(std::string key, glm::vec3 v3)
{
    ShaderValue v = { ShaderValueType::VEC3, key, v3 };
    m_Caches[key] = v;
}
void ShaderUniformsCache::Cache(std::string key, glm::vec4 v4)
{
    ShaderValue v = { ShaderValueType::VEC4, key, v4 };
    m_Caches[key] = v;
}
void ShaderUniformsCache::Cache(std::string key, int i)
{
    ShaderValue v = { ShaderValueType::INT, key, i };
    m_Caches[key] = v;
}
void ShaderUniformsCache::Cache(std::string key, unsigned int i)
{
    ShaderValue v = { ShaderValueType::INT, key, i };
    m_Caches[key] = v;
}
void ShaderUniformsCache::Cache(std::string key, bool b)
{
    ShaderValue v = { ShaderValueType::BOOL, key, b };
    m_Caches[key] = v;
}
void ShaderUniformsCache::Cache(std::string key, glm::mat4 m)
{
    ShaderValue v = { ShaderValueType::MAT, key, m };
    m_Caches[key] = v;
}

std::unordered_map<std::string, ShaderValue>& ShaderUniformsCache::GetCaches()
{
    return m_Caches;
}

void ShaderUniformsCache::Clear()
{
    m_Caches.clear();
}

void ShaderUniformsCache::FlushToShader(Shader* s)
{
    for (auto& kv : m_Caches)
    {
        std::string key = kv.first;
        ShaderValue value = kv.second;
        if (value.type == ShaderValueType::BOOL)
        {
            bool v = std::any_cast<bool>(value.value);
            s->setUniform1i(key, v);
        }

        if (value.type == ShaderValueType::FLOAT)
        {
            float v = std::any_cast<float>(value.value);
            s->setUniform1f(key, v);
        }

        if (value.type == ShaderValueType::INT)
        {
            int v = std::any_cast<int>(value.value);
            s->setUniform1i(key, v);
        }

        if (value.type == ShaderValueType::MAT)
        {
            glm::mat4 v = std::any_cast<glm::mat4>(value.value);
            s->setUniform4m(key, v);
        }

        if (value.type == ShaderValueType::VEC2)
        {
            glm::vec2 v = std::any_cast<glm::vec2>(value.value);
            s->setUniform2f(key, v.x, v.y);
        }

        if (value.type == ShaderValueType::VEC3)
        {
            glm::vec3 v = std::any_cast<glm::vec3>(value.value);
            s->setUniform3f(key, v.x, v.y, v.z);
        }

        if (value.type == ShaderValueType::VEC4)
        {
            glm::vec4 v = std::any_cast<glm::vec4>(value.value);
            s->setUniform4f(key, v.x, v.y, v.z, v.w);
        }
    }
}