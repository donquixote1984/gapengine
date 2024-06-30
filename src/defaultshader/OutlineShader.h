#pragma once
#include "AdditionalShader.h"
#include "../shaders/Shader.h"
#include <memory>
class OutlineShader: public AdditionalShader
{
private:
    Shader *m_OutlineShader;
public: 
    OutlineShader();
    ~OutlineShader();
    void Render();
    void OnUpdateMVP(glm::mat4 model, glm::mat4 view, glm::mat4 projection, glm::vec3 camPos);
};