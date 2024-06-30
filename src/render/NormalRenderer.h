#pragma once
#include "ObjectRenderer.h"
#include "../shaders/Shader.h"
#include "../geometry/Geometry.h"
class NormalRenderer: public ObjectRenderer
{
private:
    Shader *m_Shader = nullptr;
    Geometry *m_Geo = nullptr;
public:
    NormalRenderer();
    ~NormalRenderer();
    void OnRender(GLFWwindow *window);
    void OnRenderMVP(GLFWwindow *window);
    void SetGeo(Geometry * geo);
    void OnRenderGeos(std::vector<Geometry*> &geos, GLFWwindow *window);
    void OnRenderGeo(Geometry* geo, GLFWwindow *window);
};

