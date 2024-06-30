#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "stb/stb_image.h"
#include "../shaders/Shader.h"
#include "../exceptions/ResourceNotFoundException.h"

#include "../global/ScreenProps.h"
#include "../render/RenderContext.h"
#include "EnvBox.h"
#include "../Config.h"
#include "../FrameBuffer.h"

class HDRIBox: public EnvBox 
{
private:
    const int MAX_MIP_LVLS = 5;
    const int HDRI_RES = 1024;
    const int IR_RES = 32;
    const int PREFILTER_RES = 128;
    const int BRDF_RES = 512;
    std::string m_HdriFile;
    
    unsigned int m_HDRITextureId;
    unsigned int m_CubeMapTextureId;
    unsigned int m_IRMapTextureId;
    unsigned int m_PrefilterTextureId;
    unsigned int m_BRDFTextureId;
    unsigned int m_CubeMapFrameBuffer;
    unsigned int m_CubeMapRenderBuffer;

    Shader *m_HDRIShader = nullptr;
    Shader *m_IRShader = nullptr;
    Shader *m_PreFilterShader = nullptr;
    Shader *m_BRDFShader = nullptr;

    FrameBuffer *m_BRDFFb = nullptr;

    glm::mat4 m_CaptureProjection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
    glm::mat4 m_CaptureViews[6] =
    {
        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)),
        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f))
    };
public:
    HDRIBox(std::string & hdriFile);
    ~HDRIBox();
    void RenderToCubeMap();
    void RenderToIRMap();
    void RenderToPrefilterMap();
    void RenderToBRDFMap();
    void RenderCube();
    void BindTexture(int slot = 0) override;
    void ActiveEnvironment() override;

    void RenderTestBRDF();
};