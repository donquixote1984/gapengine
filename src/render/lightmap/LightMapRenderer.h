#pragma once
#include "GLFW/glfw3.h"
#include "../ObjectRenderer.h"
#include "../../shaders/Shader.h"
#include "../RenderContext.h"
#include "../../geometry/Empty.h"
#include "../../geometry/Geometry.h"
#include "../../lights/Light.h"
#include "../../lights/LightCollection.h"
#include "LightMap.h"
#include "PointLightMap.h"
#include "DirectionalLightMap.h"
#include "../../global/ScreenProps.h"
#include "../../exceptions/JumpException.h"
#include "../../lights/LightCollection.h"
#include "PointLightMapArray.h"
#include "DirectionalLightMapArray.h"
#include "SunLightMapCSM.h"
#include "SunLightMapCSM.h"
#include "../../lights/SunLight.h"
#include "../../exceptions/JumpException.h"


class LightMapRenderer: public ObjectRenderer
{
protected:
    //Light * m_Light;
    LightCollection * m_Lights;
    LightMapArray *m_PointLightMapArray = nullptr;
    LightMapArray *m_DirectionalLightMapArray = nullptr;
    SunLightMapCSM *m_SunLightMap = nullptr;
    void OnRenderSingleLightMap(LightMapArray *lma, std::vector<Geometry*> &geos, GLFWwindow * window);
    
public:
    LightMapRenderer();
    ~LightMapRenderer();
    void OnRenderLightMap(std::vector<Geometry*> geos, GLFWwindow *window);
    void OnRenderShadowSpace(GLFWwindow *window);
    void SetSceneLights(LightCollection* );
    void ActiveTexture();
    void RenderGeometry(Geometry *g) const;
    void EnableSunLight(Light * light);
};