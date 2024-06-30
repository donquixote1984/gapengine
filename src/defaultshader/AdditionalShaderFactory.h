#pragma once
#include "DefaultShaderType.h"
#include "AdditionalShader.h"
#include "OutlineShader.h"

class AdditionalShaderFactory
{
public:
    static AdditionalShader* GetShader(DefaultShaderType type)
    {
        switch (type)
        {
        case DefaultShaderType::OUTLINE:
            return new OutlineShader();
        default:
            return new AdditionalShader();
        }
        return new AdditionalShader();
    }
};