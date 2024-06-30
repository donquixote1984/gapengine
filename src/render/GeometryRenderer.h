#pragma once
#include "GLFW/glfw3.h"
#include <vector>
#include <array>
#include "../shaders/Shader.h"
#include "ObjectRenderer.h"
#include "../shaders/ShaderType.h"
#include "Material.h"
#include "RenderContext.h"
#include "../skybox/EnvBox.h"
#include "../geometry/DisplayMode.h"
#include "../geometry/Geometry.h"
#include "../shaders/GeometryShader.h"
#include "fmt/format.h"
#include "../ui/UISettings.h"

class GeometryRenderer: public ObjectRenderer
{
protected:
    Geometry *m_Geo = nullptr;
    EnvBox *m_EnvBox = nullptr;
    void OnRenderMisc(GLFWwindow *window);
    void OnRenderMaterial();
public:
    void OnRender(GLFWwindow *window);

    GeometryRenderer();
    GeometryRenderer(RenderContext &rc);
    void SetGeo(Geometry * geo);
    void SetEnvBox(EnvBox * envbox);
    void OnRenderGeos(std::vector<Geometry*> &geos, GLFWwindow *window);
    void OnRenderGeo(Geometry* geo, GLFWwindow *window);
};