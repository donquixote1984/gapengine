#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <memory>
#include "Geometry.h"
#include "GeometryInstancingDataProcessor.h"
#include "GeometryInstancingType.h"
#include "../Transform.h"
#include "../shaders/ShaderSnippet.h"
#include "../UniformBuffer.h"


class GeometryInstancing: public Geometry
{
private:
protected:
    int m_InstancingCount = 0;
    GeometryInstancingType m_InstancingType = GeometryInstancingType::STATIC;
    //glm::mat4 *m_InstancingMVP = nullptr;
    std::shared_ptr<Transform[]> m_InstancingMVP;
    void InitGeoData() override;
public:
    void SetMaterial(Material *m) override;
    GeometryInstancing(GeometryType gType);
    GeometryInstancing(GeometryType gtype, GeometryInstancingType itype);
    void SetInstancing(std::shared_ptr<Transform[]> mvps, unsigned int count);
    void SetInstancingCount(int count);
    GeometryInstancingType GetInstancingType();
    int GetInstancingCount();
    Transform* GetInstancingMVP();
    void ResolveMeta() override;
    bool IsInstancing() override;
    void InitProcessor() override;
    void ReadyToScene() override;
};