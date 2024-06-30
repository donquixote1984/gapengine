#pragma once
#include <glad/glad.h>
#include "../shaderstructures/ShaderStructures.hpp"
#include "../UniformBuffer.h"
namespace settings {

    enum class LightMode
    {
        BLINN,
        PHONG
    };

    class UISettings
    {
    private:
        UISettings();
        ~UISettings();
        UISettings(const UISettings &us) = delete;
        
    public:
        int lightMode = 0;
        int gamma = 0;
        int shadow = 1;
        int PCF = 0;
        int csmlayer = 0;

        int envlight = 1;
        int verticesCount = 0;
        int normal = 1;
        static UISettings* GetSettings();
        void ToggleGamma();
        void FlushToShader();
    };
}
