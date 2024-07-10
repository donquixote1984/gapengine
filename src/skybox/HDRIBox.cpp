#include "HDRIBox.h"
#include "../FrameBufferDebugger.h"
HDRIBox::HDRIBox(std::string & hdriFile): m_HdriFile(hdriFile)
{
    glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
    stbi_set_flip_vertically_on_load(true);
    int width, height, nrChannels;
    float *data = stbi_loadf(hdriFile.c_str(), &width, &height, &nrChannels, 0);
    if (data)
    {
        glGenTextures(1, &m_HDRITextureId);
        glBindTexture(GL_TEXTURE_2D, m_HDRITextureId);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, data); // note how we specify the texture's data value to be float

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        char * msg;
        sprintf(msg, "hdri not found: %s", hdriFile.c_str());
        throw ResourceNotFoundException(msg);
    }

    m_HDRIShader = new Shader("res/shaders/skybox/hdri.shader");
    m_HDRIShader->Nohup();
    m_IRShader   = new Shader("res/shaders/skybox/ir.shader");
    m_IRShader->Nohup();
    m_PreFilterShader = new Shader("res/shaders/skybox/prefilter.shader");
    m_PreFilterShader->Nohup();
    m_BRDFShader = new Shader("res/shaders/skybox/brdf.shader");
    m_BRDFShader->Nohup();


    glGenFramebuffers(1, &m_CubeMapFrameBuffer);
    glGenRenderbuffers(1, &m_CubeMapRenderBuffer);

    RenderToCubeMap();
    RenderToIRMap();
    RenderToPrefilterMap();
    RenderToBRDFMap();
    m_HDRIShader->UnBind();
}

void HDRIBox::RenderToCubeMap()
{

    glGenTextures(1, &m_CubeMapTextureId);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_CubeMapTextureId);
    for (int i = 0; i < 6; i++)
    {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, HDRI_RES, HDRI_RES, 0, GL_RGB, GL_FLOAT, nullptr);
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); 
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


    glBindFramebuffer(GL_FRAMEBUFFER, m_CubeMapFrameBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, m_CubeMapRenderBuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, HDRI_RES, HDRI_RES);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_CubeMapRenderBuffer);

    glViewport(0, 0, HDRI_RES, HDRI_RES); 
    glBindFramebuffer(GL_FRAMEBUFFER, m_CubeMapFrameBuffer);

    m_HDRIShader->Bind();
    m_HDRIShader->setUniform1i("hdriMap", 0);
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_HDRITextureId);

    m_HDRIShader->setUniform4m("projection", m_CaptureProjection);
    for (int i = 0; i < 6; i++)
    {
        m_HDRIShader->setUniform4m("view", m_CaptureViews[i]);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, m_CubeMapTextureId, 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        RenderCube();
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_CubeMapTextureId);
    glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
    glViewport(0, 0, Global::screen.width, Global::screen.height);
}

void HDRIBox::RenderToIRMap()
{
    glGenTextures(1, &m_IRMapTextureId);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_IRMapTextureId);
    for (int i = 0; i < 6; i++)
    {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, IR_RES, IR_RES, 0, GL_RGB, GL_FLOAT, nullptr); // set to 32 x 32 ?!
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindFramebuffer(GL_FRAMEBUFFER, m_CubeMapFrameBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, m_CubeMapRenderBuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, IR_RES, IR_RES);

    glViewport(0, 0, IR_RES, IR_RES); 
    m_IRShader->Bind();
    m_IRShader->setUniform1i("skybox", 0);
    m_IRShader->setUniform4m("projection", m_CaptureProjection);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_CubeMapTextureId);
    for (int i = 0; i < 6; i++)
    {
        m_IRShader->setUniform4m("view", m_CaptureViews[i]);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, m_IRMapTextureId, 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        RenderCube();
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, Global::screen.width, Global::screen.height);
}

void HDRIBox::RenderToPrefilterMap()
{
    glGenTextures(1, &m_PrefilterTextureId);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_PrefilterTextureId);
    for (int i = 0; i < 6; i++)
    {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, PREFILTER_RES, PREFILTER_RES, 0, GL_RGB, GL_FLOAT, nullptr); 
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_CubeMapTextureId);
    m_PreFilterShader->Bind();
    m_PreFilterShader->setUniform1i("skybox", 0);
    m_PreFilterShader->setUniform4m("projection", m_CaptureProjection);

    glBindFramebuffer(GL_FRAMEBUFFER, m_CubeMapFrameBuffer);
    for (int i = 0; i < MAX_MIP_LVLS; i++)
    {
        unsigned int mipWidth = static_cast<unsigned int>(PREFILTER_RES * std::pow(0.5f, i));
        unsigned int mipHeight = mipWidth;
        glBindRenderbuffer(GL_RENDERBUFFER, m_CubeMapRenderBuffer);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, mipWidth, mipHeight);
        glViewport(0, 0, mipWidth, mipHeight);

        float roughness = (float) i / (float)(MAX_MIP_LVLS - 1);
        m_PreFilterShader->setUniform1f("roughness", roughness);

        for (int j = 0; j < 6; j++)
        {
            m_PreFilterShader->setUniform4m("view", m_CaptureViews[j]);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + j, m_PrefilterTextureId, i);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            RenderCube();
        }
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, Global::screen.width, Global::screen.height);

}

void HDRIBox::RenderToBRDFMap()
{
    glGenTextures(1, &m_BRDFTextureId);
    glBindTexture(GL_TEXTURE_2D, m_BRDFTextureId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RG16F, BRDF_RES, BRDF_RES, 0, GL_RG, GL_FLOAT, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindFramebuffer(GL_FRAMEBUFFER, m_CubeMapFrameBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, m_CubeMapRenderBuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, BRDF_RES, BRDF_RES);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_BRDFTextureId, 0);
    glViewport(0, 0, BRDF_RES, BRDF_RES);
    m_BRDFShader->Bind();
    m_BRDFShader->Statistic();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    m_ScreenCanvas.Draw();
    //FrameBufferDebugger::get()->SetCurrentFrameBuffer(m_ReflectionFrameBuffer);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void HDRIBox::RenderCube()
{
    glActiveTexture(GL_TEXTURE0);
    m_VAO.Bind();
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
}

HDRIBox::~HDRIBox()
{
    delete m_HDRIShader;
    delete m_BRDFShader;
    delete m_PreFilterShader;
    delete m_BRDFFb;

    m_HDRIShader = nullptr;
    m_BRDFShader = nullptr;
    m_PreFilterShader = nullptr;
    m_BRDFFb = nullptr;

    glDeleteFramebuffers(1, &m_CubeMapFrameBuffer);
    glDeleteRenderbuffers(1, &m_CubeMapRenderBuffer);
    glDeleteTextures(1, &m_CubeMapTextureId);
    glDeleteTextures(1, &m_HDRITextureId);
    glDeleteTextures(1, &m_IRMapTextureId);
    glDeleteTextures(1, &m_PrefilterTextureId);
    glDeleteTextures(1, &m_BRDFTextureId);
}

void HDRIBox::BindTexture(int slot)
{
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_CubeMapTextureId);
    //glBindTexture(GL_TEXTURE_CUBE_MAP, m_IRMapTextureId);
    //glBindTexture(GL_TEXTURE_CUBE_MAP, m_PrefilterTextureId);
}


void HDRIBox::ActiveEnvironment()
{
    glActiveTexture(GL_TEXTURE0 + constants::IR_MAP);  // 24slot => irmap
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_IRMapTextureId);

    glActiveTexture(GL_TEXTURE0 + constants::PREFILTER_MAP);  // 25slot => prefiltermap
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_PrefilterTextureId);

    //m_BRDFFb->ActiveTexture(constants::BRDFLUT_MAP);
    glActiveTexture(GL_TEXTURE0 + constants::BRDFLUT_MAP);
    glBindTexture(GL_TEXTURE_2D, m_BRDFTextureId);
    
}

void HDRIBox::RenderTestBRDF()
{
    m_BRDFFb->FlushToScreen();
}