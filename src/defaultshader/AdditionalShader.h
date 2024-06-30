#pragma once
#include <glm/glm.hpp>
#include <iostream>
#include <vector>
#include "DefaultShaderType.h"
#include "../geometry/Geometry.h"

class AdditionalShader
{
private:
    std::vector<void (*)(void)> m_Ops;
protected:
    Geometry *m_TargetGeo;
public:
    AdditionalShader();
    virtual ~AdditionalShader();
    void PreHandle();
    void PostHandle();
    virtual void Render();
    void AddOperation(void (*)(void));
    void AddTargetGeo(Geometry*);
    void AddDrawCall(void (*));
    virtual void OnUpdateMVP(glm::mat4 model, glm::mat4 view, glm::mat4 projection, glm::vec3 camPos);
};