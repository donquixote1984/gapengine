#pragma once
#include <string>
#include "lua.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <unordered_map>
#include <vector>

const std::string SCRIPT_CONTEXT_ROOT = "obj";

struct LuaQuat
{
    glm::quat m_Quat;

    void Init(float angle, float x, float y, float z)
    {
        m_Quat = glm::angleAxis(glm::radians(angle), glm::vec3(x, y, z));
    }
    void Multiply(glm::quat *q)
    {
        m_Quat = (*q) * m_Quat;
    }
    void Multiply(LuaQuat *q)
    {
        m_Quat = q->m_Quat * m_Quat;
    }
};
struct ScriptContext
{
    std::unordered_map<std::string, std::string> context;
 //   std::vector<std::string> context;
    glm::vec3 *position = NULL;
    glm::quat *rotation = NULL;
    glm::vec3 *scale    = NULL;

    ScriptContext()
    {
       // context = {};
    }
    ScriptContext(const ScriptContext& context) = delete;
};

class ScriptContextBinding
{
public:

    // transform
    static int MetatableIndexTransform(lua_State *L)
    {
        struct ScriptContext * t = (ScriptContext *) luaL_checkudata(L, 1, "transform");
        std::string key = luaL_checkstring(L, 2);
        if (key == "position")
        {
            lua_pushlightuserdata(L, t->position);
        }

        if (key == "rotation")
        {
            lua_pushlightuserdata(L, t->rotation);
        }

        if (key == "scale")
        {
            lua_pushlightuserdata(L, t->scale);
        }
        return 1;
    }

    static int MetatableNewIndexTransform(lua_State *L)
    {
        ScriptContext* c = (ScriptContext*) luaL_checkudata(L, 1, "transform");
        std::string key = luaL_checkstring(L, 2); // right
        LuaQuat *value = (LuaQuat *)(lua_touserdata(L, 3)); // right.
        if (key == "rotation")
        {
            *c->rotation = value->m_Quat;
        }
        return 1;
        return 1;
    }


    // vector
    static int MetatableIndexVector(lua_State *L)
    {
        glm::vec3 * t = (glm::vec3*) luaL_checkudata(L, 1, "vector");
        std::string key = luaL_checkstring(L, 2);
        if (key == "x")
        {
            lua_pushnumber(L, t->x);
        }
        if (key == "y")
        {
            lua_pushnumber(L, t->y);
        }

        if (key == "z")
        {
            lua_pushnumber(L, t->z);
        }
        return 1;
    }

    static int MetatableNewIndexVector(lua_State *L)
    {
        glm::vec3 * t = (glm::vec3 *) luaL_checkudata(L, 1, "vector");
        std::string key = luaL_checkstring(L, 2);
        float value = (float)lua_tonumber(L, 3);

         if (key == "x")
        {
            t->x = value;
        }
        if (key == "y")
        {
            t->y = value;
        }

        if (key == "z")
        {
            t->z = value;
        }
        return 1;
    }

    // context map


    static void CreateTransformBinding(lua_State *L)
    {
        luaL_newmetatable(L, "transform");
        lua_pushcfunction(L, MetatableIndexTransform);
        lua_setfield(L, -2, "__index");
        lua_pushcfunction(L, MetatableNewIndexTransform);
        lua_setfield(L, -2, "__newindex");
    }

    static void CreateVectorBinding(lua_State *L)
    {
        luaL_newmetatable(L, "vector");
        lua_pushcfunction(L, MetatableIndexVector);
        lua_setfield(L, -2, "__index");
        lua_pushcfunction(L, MetatableNewIndexVector);
        lua_setfield(L, -2, "__newindex");   
    }

    static int QuatNew(lua_State *L)
    {
        float angle = (float)luaL_checknumber(L, 1);
        float x = (float)luaL_checknumber(L, 2);
        float y = (float)luaL_checknumber(L, 3);
        float z = (float)luaL_checknumber(L, 4);

        LuaQuat *t = (LuaQuat *)lua_newuserdata(L, sizeof(LuaQuat));
        t->Init(angle, x, y, z); 
        luaL_setmetatable(L, "Quat");
        return 1;
    }

    static int QuatMultiply(lua_State * L)
    {
        if(lua_islightuserdata(L, 2))
        {
            // multiply rotate;
            glm::quat * d = (glm::quat *)lua_touserdata(L, 2);
            LuaQuat *q = (LuaQuat *)luaL_checkudata(L, 1, "Quat");
            q->Multiply(d);
        } 
        if(luaL_testudata(L, 2, "Quat"))
        {
            LuaQuat *q2 = (LuaQuat *)luaL_checkudata(L, 2, "Quat");
            LuaQuat *q = (LuaQuat *)luaL_checkudata(L, 1, "Quat");
            q->Multiply(q2);
        }
        lua_pop(L, 1);
        // (*reinterpret_cast<Quat**>(luaL_checkudata(L, 1, "Quat")))->Multiply(c->rotate);
        return 1;
    }

    static void CreateQuatBinding(lua_State *L)
    {
        lua_register(L, "Quat", QuatNew);
        luaL_newmetatable(L, "Quat");
        lua_pushcfunction(L, QuatMultiply);
        lua_setfield(L, -2, "__mul");
    }

    static ScriptContext * CreateBinding(lua_State *L)
    {
        CreateTransformBinding(L);
        CreateVectorBinding(L);
        CreateQuatBinding(L);
        ScriptContext *sc = (ScriptContext*) lua_newuserdata(L, sizeof(ScriptContext));
        luaL_setmetatable(L, "transform");
        lua_setglobal(L, "obj");

        lua_getglobal(L, "obj");
        lua_getfield(L, -1, "position");
        luaL_setmetatable(L, "vector");

        lua_getglobal(L, "obj");
        lua_getfield(L, -1, "rotation");
        luaL_setmetatable(L, "vector");

        lua_getglobal(L, "obj");
        lua_getfield(L, -1, "scale");
        luaL_setmetatable(L, "vector");

        return sc;
    }
};