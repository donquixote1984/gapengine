#pragma once
#include <glm/glm.hpp>
#include "LightMapArray.h"
#include "../../lights/PointLight.h"

#include <vector>
#include <utility>

class PointLightMapArray: public LightMapArray
{
private:
    void InitShaders() override;
    std::pair<glm::vec3, glm::vec3> m_LookAts[6] = {
        {
            glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)
        },
        {
            glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)
        },
        {
            glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)
        },
        {
            glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f)
        },
        {
            glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f)
        },
        {
            glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f)
        }
    };
public:
    PointLightMapArray();
    ~PointLightMapArray();
    void Bind() override;
    void ActiveTexture() override;
    void OnRenderLightSpace() override;
    void RenderDebug(Geometry*g) override;
    bool HasLights() override;
};