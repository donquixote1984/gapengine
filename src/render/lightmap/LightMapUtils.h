#pragma once

#include <glm/glm.hpp>
#include <vector>
#include "../../Camera.h"
#include "../../geometry/Geometry.h"
#include "../../lights/Light.h"

class LightMapUtils
{
public:
    static glm::mat4 GetLightSpaceMatrix(Camera * camera, Light* l, float nearPlane, float farPlane)
    {
        //float w = Global::screen.width;
        //float h = Global::screen.height;
        //const auto proj = glm::perspective(glm::radians(m_Camera.GetCameraSettings().zoom), (float)(w / h), m_Camera.GetCameraSettings().nearPlane, m_Camera.GetCameraSettings().farPlane);
        //glm::vec3 corners = 
        const auto corners = camera->GetFrustumCornersWorldSpace(nearPlane, farPlane);
        glm::vec3 frustrumCenter = corners.Center();
        const auto lightView = glm::lookAt(frustrumCenter - glm::normalize(l->GetDirection()), frustrumCenter, glm::vec3(.0f, 1.0f, .0f));

        float maxX= std::numeric_limits<float>::lowest();
        float minX= std::numeric_limits<float>::max();

        float maxY= std::numeric_limits<float>::lowest();
        float minY= std::numeric_limits<float>::max();

        float maxZ= std::numeric_limits<float>::lowest();
        float minZ= std::numeric_limits<float>::max();

        for (int i = 0; i < 8; i++)
        {
            const auto pos = lightView * corners[i];
            minX = std::min(pos.x, minX);
            maxX = std::max(pos.x, maxX);

            minY = std::min(pos.y, minY);
            maxY = std::max(pos.y, maxY);

            minZ = std::min(pos.z, minZ);
            maxZ = std::max(pos.z, maxZ);
        }

            constexpr float zMult = 10.0f;
        if (minZ < 0)
        {
            minZ *= zMult;
        }
        else
        {
            minZ /= zMult;
        }
        if (maxZ < 0)
        {
            maxZ /= zMult;
        }
        else
        {
            maxZ *= zMult;
        }

        const glm::mat4 lightProjection = glm::ortho(minX, maxX, minY, maxY, minZ, maxZ);
        return lightProjection * lightView;
    }

    static void OnRenderGeometryCSM(Geometry *g, Camera *camera, std::vector<float> &shadowCascadeLevels, bool useCSM)
    {
        Shader * s = g->GetMaterial()->GetShader();
        s->Bind();
        //s->setUniform1i("hasShadowMap", 1);
        s->setUniform1i("u_CascadeShadowMap", constants::CSM);
        //s->setUniform4m("lightSpaceMatrix", this->GetLightSpaceMat(l));
        if (useCSM) {
            for (int i = 0 ; i < shadowCascadeLevels.size(); i ++)
            {
                s->setUniform1f("u_CascadePlaneDistances[" +std::to_string(i)+ "]", shadowCascadeLevels[i]);
            }
        } else {
            s->setUniform1f("u_CascadePlaneDistances[0]", camera->GetCameraSettings().farPlane);
        }
    }

};