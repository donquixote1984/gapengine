#pragma once
#include <string>
#include <iostream>
#include "lua.hpp"
#include "GLFW/glfw3.h"
#include "ScriptContext.h"

class GeometryScript
{
private:
    std::string m_Script;
    lua_State * L;
    ScriptContext *m_Context;
    float deltaTime;
    float lastFrame;
public:
    GeometryScript(const std::string &script);
    ~GeometryScript();
    void OnFrame(GLFWwindow *window);
    void ResetContext(ScriptContext &sc);
    ScriptContext * GetContext();
};