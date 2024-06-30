#include "GeometryScript.h"

GeometryScript::GeometryScript(const std::string &script):m_Script(script)
{
    L = luaL_newstate();
    luaL_openlibs(L);
    m_Context = ScriptContextBinding::CreateBinding(L);
    int ret = luaL_loadstring(L, script.c_str());
    if (ret)
    {
        std::cout << lua_tostring(L, -1)<<std::endl;
    }
    lua_setglobal(L, "script");
    //luaL_dostring(L, script.c_str());
}

GeometryScript::~GeometryScript()
{
    lua_close(L);
}

void GeometryScript::OnFrame(GLFWwindow *window)
{
    float currentFrame = static_cast<float>(glfwGetTime());
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
    //luaL_loadstring()
    //luaL_dostring(L, m_Script.c_str());
    lua_pushnumber(L, deltaTime);
    lua_setglobal(L, "deltaTime");
    lua_getglobal(L, "script");
    lua_pcall(L, 0, 0, 0);
    //luaL_dostring(L, m_Script.c_str());

}
ScriptContext * GeometryScript::GetContext()
{
    return m_Context;
}
void GeometryScript::ResetContext(ScriptContext &sc)
{
    m_Context->position = sc.position;
    m_Context->rotation = sc.rotation;
    m_Context->scale    = sc.scale;
    //m_Context->context  = std::unordered_map<std::string, std::string>();
}