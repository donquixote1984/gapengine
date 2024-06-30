#pragma once
#include <glad/glad.h>
class DefaultShaderOperationUtils
{
public:
    static void EnableGlobalStencil()
    {
        glEnable(GL_STENCIL_TEST);
    }
    static void EnableStencil()
    {
        glStencilMask(0xFF);
    }
    static void ReplaceStencilWhenAllPass()
    {
        glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
    }
    static void SetCurrentObjAsStencil()
    {
        EnableStencil();
         glStencilFunc(GL_ALWAYS, 1, 0xFF);
    }
    static void OutputIfNotStencil()
    {
        glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
    }
    static void OutputIfStencil()
    {
        glStencilFunc(GL_EQUAL, 1, 0xFF);
    }
    static void DisableStencil()
    {
        glStencilMask(0x00);
    }
    static void ClearStencil()
    {
        glStencilMask(0xFF);
        glStencilFunc(GL_ALWAYS, 0, 0xFF);
    }

};