#pragma once
#include <malloc.h>
#include <vector>
#include "Light.h"
#include "PointLight.h"
#include "DirectionalLight.h"
#include "SunLight.h"
#include "../shaderstructures/ShaderStructures.hpp"
#include "../UniformBuffer.h"

class Scene;

class LightCollection
{
friend class Scene; // only scene can change light.
private:
    std::vector<Light*> m_Lights;
    std::vector<PointLight *> m_PointLights;
    std::vector<DirectionalLight *> m_DirectionalLights;
    SunLight * m_SunLight = nullptr;
    void AddLight(Light * l);
    void RemoveLight(Light * l); 
     ~LightCollection();
public:
    std::vector<Light*> GetLights() const;
    std::vector<PointLight*> GetPointLights() const;
    void SerializePointLights();
    void SerializeDirLights();
    void SerializeSunLight();
    void SerializeLights();
    std::vector<DirectionalLight*> GetDirectionalLights() const;
    bool HasDirectionalLights() const;
    bool HasPointLights() const;
    bool HasSunLight() const;
};
