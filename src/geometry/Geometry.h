#pragma once

#include "GLFW/glfw3.h"
#include <vector>
#include <string>
#include <initializer_list>
#include <glm/glm.hpp>
#include <unordered_map>
#include <glm/gtc/matrix_transform.hpp>
#include "../exceptions/GeometryInitializeException.h"
#include "../Mesh.h"
#include "../Vertex.h"
#include "../VertexArray.h"
#include "../IndexBuffer.h"
#include "../defaultshader/DefaultShaderType.h"
#include "../shaders/ShaderType.h"
#include "../Config.h"
#include "../util/Util.h"
#include "../render/Material.h"
#include "../render/SharedMaterial.h"
#include "Empty.h"
#include "GeometryData.h"
#include "GeometryType.h"
#include "postprocess/DataPostProcessing.h"
#include "GeometryDataProcessor.h"
#include "DisplayMode.h"
#include "GeometryMeta.h"
#include "../script/GeometryScript.h"
#include "../UniformBuffer.h"
#include "../animation/Animator.h"


class DataProcessor;

class Geometry: public ShadedEmptyTransform 
{
    friend class GeometryDataProcessor;
private:
protected:
  //  std::vector<unsigned int> m_Indices;
    //GeometryRender *render;
    //IndexBuffer m_EBO;
    
    GeometryData *m_GeoData = NULL;  // it is a array, size MAX_GEO_VARIANT
    bool m_HasVariant = false;
    uint8_t m_DataVariantNum = 0;
    //Texture m_DiffuseTex;
    //Texture m_SpecularTex;
    std::vector<Mesh> m_Meshes;
    
    GeometryMeta m_Meta;
    
    DataProcessor *m_Dp = nullptr;
    GeometryType m_Type;
    virtual void InitGeoData();
    bool m_ShadowPass = false;
    bool m_Playing = false;
    Animator m_Animator;
public:
    Geometry(GeometryType geoType);
    //Geometry(float *vertices, unsigned int size, std::initializer_list<int> layouts);
    ~Geometry();
    Geometry(const Geometry & g) = delete;
    void OnUpdateRender(GLFWwindow *window);
    void InitGeometry(const std::vector<Vertex> &vertices, const std::vector<unsigned int> &indices);
    void InitGeometryData(float * vertices, unsigned int count, std::initializer_list<int> layouts);
    void InitGeometryData(std::shared_ptr<float[]> data, unsigned int count,  std::initializer_list<int> layouts);
    void InitGeometryData(GeometryData *data, uint8_t variant = 1);
    void InitGeometryVariant(GeometryData * data, uint8_t variant);
    virtual void InitMaterial();
    void PreRender() override;
    void InitVAO();
   
    void SetScale(glm::vec3 scale);
    
    void SetDisplayMode(DisplayMode mode);
    void AddPostProcessing(DataPostProcessing * dpp);
    DisplayMode GetDisplayMode() const;
    void RenderDrawCall() override;
    
    
    virtual void ProcessData();
    void SwitchMaterial(Material *m);
    
    void AttachMeta(GeometryMeta meta);
    virtual void ResolveMeta();
    GeometryMeta & GetMeta();
    
    int GetVerticesCount();
    GeometryData *GetGeoData();
    DataProcessor * GetDataProcessor();
    bool IsInstancing() override;
    virtual void InitProcessor();
    void ScriptCallback();
    void AttachScript(const std::string &script) override;
    bool IsEmpty() override;
    void InitPasses() override;
    void SetShadowPass(bool shadowPass);
    bool IsGeometry() override;
    virtual bool HasTess();
    void MoveEmbedTexturesToMaterial();
    bool IsPlaying();
    void SetPlaying(bool playing);
    void ReadyToScene() override;

    Animator * GetAnimator();
    void PlayAnimation(unsigned int animationIndex = 0);
    void FeedAnimations(std::vector<SceneAnimation *>);
    
};
