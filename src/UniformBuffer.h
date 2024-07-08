#pragma once
#include <iostream>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <initializer_list>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include "exceptions/WrongAccumulationInRenderLoopException.h"
#include "Config.h"
#include "shaderstructures/ShaderStructures.hpp"
const int MAX_CSM_SIZE = 6;
const int MAX_PSM_SIZE = 6;
enum class UBOBindings
{
    //layout uniform Matrices {
    // glm::mat4 projection
    // glm::mat4 view
    // ... preserve 5 matrix
    //}
    MATRICES = 1,  

    //layout uniform PSM {
    // glm::mat4[n plights][6];
    //}
    PSM = 2,
    // layout uniform CSM {
    // glm::mat4[n dlights][CSM_NUMBER = 5]
    //}
    CSM = 3,
    LIGHTS = 4,
    SETTINGS = 5,
    SUN_CSM = 6,
    BONES = 7,
};

struct UniformBufferBindingSlot
{
    unsigned int slot;
    unsigned int offset = 0;
    //size_t size;
    std::string channel;
    size_t preservedSize;

    bool operator==(const UniformBufferBindingSlot& lhs) const
    {
        if (slot == lhs.slot && channel == lhs.channel)
        {
            return true;
        }

        return false;
    }
   
};

class UniformBuffer
{
protected:
    unsigned int m_UBO;
    unsigned int m_Size = 40960;
    unsigned int m_CurrentOffset = 0;
    unsigned int m_BlockOffset = 0;
    int m_UniformBufferAlignSize = 0;
    unsigned int slot = 0;
    const int MAX_OFFSET = 1000;
    UniformBuffer();
    ~UniformBuffer();
    UniformBuffer(const UniformBuffer&) = delete;
public:
    void Feed(glm::mat4 &mat);
    void Feed(glm::mat4 *mat4, unsigned int offset);
    void Feed(glm::mat4 *matArrayHeader, unsigned int offset, unsigned int matArraySize);
    void Feed(int num, int offset);
    void Reset();
    void Bind();
    void BindShaderSlot(unsigned int shaderId, const char *, int slot, size_t size);
    void BindShaderSlot(unsigned int shaderId, UniformBufferBindingSlot &slot);
};

class MatricesUniformBuffer : public UniformBuffer
{
protected:
public:
    static MatricesUniformBuffer* GetUniformBuffer();
};

class LightUniformBuffer: public UniformBuffer
{
public:
    void Feed(ShaderDirLight *l, int count, int offset);
    void Feed(ShaderPointLight *l, int count, int offset);
    void Feed(ShaderSunLight *l, int offset);
    void Feed(int num, int offset);
    static LightUniformBuffer* GetUniformBuffer();
};

class BoneUniformBuffer : public UniformBuffer
{
public:
    void Feed(glm::mat4* bones, int boneNum);
    static BoneUniformBuffer* GetUniformBuffer();
};

class SettingsUniformBuffer: public UniformBuffer
{
public:
    static SettingsUniformBuffer * GetUniformBuffer();
};

class MatricesUniformBufferBindings
{
private:
    static const unsigned int PRESERVE_MAT_SIZE = 10; // 10 universal mat
    static const unsigned int PRESERVE_CSM_SIZE = 10; // 10 directional lights, with each 5 csm
    static const unsigned int PRESERVE_PSM_SIZE = 10; // 10 point lights 
    //static const unsigned int PERSERVE_SUN_SIZE = 1;  // sun light, with 5 csm

    inline static bool hasCheckedOffset = false;
    static void DetermineOffset() {
        if (hasCheckedOffset) {
            return;
        }
        hasCheckedOffset = true;
        int uniformBufferAlignSize = 0;
        glGetIntegerv(GL_UNIFORM_BUFFER_OFFSET_ALIGNMENT, &uniformBufferAlignSize); // 256 
        MatricesSlot.offset = 0 ;
        CSMSlot.offset = uniformBufferAlignSize * (int)(std::floor(MatricesSlot.preservedSize / uniformBufferAlignSize) + 1);
        PSMSlot.offset = uniformBufferAlignSize * (int)(std::floor((CSMSlot.offset + CSMSlot.preservedSize) / uniformBufferAlignSize) + 1);
        SUNSlot.offset = uniformBufferAlignSize * (int)(std::floor((PSMSlot.offset + PSMSlot.preservedSize) / uniformBufferAlignSize) + 1);
    }
public:
    inline static UniformBufferBindingSlot MatricesSlot = {
        (unsigned int)UBOBindings::MATRICES, 
        0, 
        "Matrices", 
        sizeof(glm::mat4) * PRESERVE_MAT_SIZE};


    inline static UniformBufferBindingSlot CSMSlot = {
        (unsigned int)UBOBindings::CSM, 
        0,
        "CSM", 
        PRESERVE_CSM_SIZE * constants::MAX_CSM_NUMBER * sizeof(glm::mat4)};

    inline static UniformBufferBindingSlot PSMSlot = {
        (unsigned int)UBOBindings::PSM, 
        0,
        "PSM", 
        PRESERVE_PSM_SIZE * 6 * sizeof(glm::mat4)
    };

    inline static UniformBufferBindingSlot SUNSlot = {
        (unsigned int)UBOBindings::SUN_CSM,
        0,
        "SUN_CSM",
        constants::MAX_CSM_NUMBER * sizeof(glm::mat4)
    };




    /*
    inline static const UniformBufferBindingSlot TEST_MVP_1 = {
        1, 0, "TEST_MVP_1", sizeof(glm::mat4)
    };

    inline static const UniformBufferBindingSlot TEST_MVP_2 = {
        2, 256, "TEST_MVP_2", sizeof(glm::mat4)
    }; */
    static void BindMatrices(unsigned int shaderId)
    {
        DetermineOffset();
        MatricesUniformBuffer::GetUniformBuffer()->BindShaderSlot(shaderId, MatricesSlot);
    }

    static void BindPSM(unsigned int shaderId)
    {
        DetermineOffset();
        MatricesUniformBuffer::GetUniformBuffer()->BindShaderSlot(shaderId, PSMSlot);
    }

    static void BindCSM(unsigned int shaderId)
    {
        DetermineOffset();
        MatricesUniformBuffer::GetUniformBuffer()->BindShaderSlot(shaderId, CSMSlot);
    }

    static void BindSUNCSM(unsigned int shaderId)
    {
        DetermineOffset();
        MatricesUniformBuffer::GetUniformBuffer()->BindShaderSlot(shaderId, SUNSlot);
    }

    static void BindSlot(unsigned int shaderId, UniformBufferBindingSlot &slot)
    {
        DetermineOffset();
        MatricesUniformBuffer::GetUniformBuffer()->BindShaderSlot(shaderId, slot);
    }
    
    static void FeedMatrices(std::initializer_list<glm::mat4 *> mats)
    {
        MatricesUniformBuffer *ub = MatricesUniformBuffer::GetUniformBuffer();
        int index = 0;
        for (glm::mat4 *mat: mats)
        {
            //glBufferSubData(GL_UNIFORM_BUFFER, MatricesSlot.offset + index * sizeof(glm::mat4),  sizeof(glm::mat4), mat);
            ub->Feed(mat, MatricesSlot.offset + index * sizeof(glm::mat4));
            index ++;
        }
    }
    static void FeedMatrices(glm::mat4 *mats, int size)
    {
        MatricesUniformBuffer *ub = MatricesUniformBuffer::GetUniformBuffer();
        ub->Feed(mats, MatricesSlot.offset, size);
    }

    static void FeedMatrices(glm::mat4 mat, int matIndex)
    {
        MatricesUniformBuffer *ub = MatricesUniformBuffer::GetUniformBuffer();
        ub->Feed(&mat, matIndex * sizeof(glm::mat4),  1);
    }

    static void FeedTest(glm::mat4 *proj, glm::mat4 *view)
    {
        MatricesUniformBuffer *ub = MatricesUniformBuffer::GetUniformBuffer();
        ub->Feed(proj, 0);
        ub->Feed(view, 256);
    }


    static void FeedPSM(glm::mat4 *mats, int lightCount)
    {
        if (lightCount > MAX_PSM_SIZE)
        {
            throw WrongAccumulationInRenderLoopException("psm size larger than thresold: 6");
        }
        MatricesUniformBuffer *ub = MatricesUniformBuffer::GetUniformBuffer();
        ub->Feed(mats, PSMSlot.offset, lightCount * 6);
    }

    static void FeedCSM(glm::mat4 * mats, int lightCount)
    {

        if (lightCount > MAX_CSM_SIZE) 
        {
            throw WrongAccumulationInRenderLoopException("csm size larger than thresold: 5");
        }
        MatricesUniformBuffer *ub = MatricesUniformBuffer::GetUniformBuffer();
        ub->Feed(mats, CSMSlot.offset, lightCount * constants::MAX_CSM_LVLS);
    }
    static void FeedSUN(glm::mat4 * mats)
    {
        MatricesUniformBuffer *ub = MatricesUniformBuffer::GetUniformBuffer();
        ub->Feed(mats, SUNSlot.offset, constants::MAX_CSM_LVLS);
    }
};



/**
 * struct PointLight {
    int index;
    float intensity;
    float constant;
    float linear;
    float quadratic;
    int castShadow;
    float farPlane;
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};  
=> offset  (7 * 4 + 4(preserve for 16*n)) + 16 * 4 = 96

struct DirLight {
    int index;
    float intensity;
    int castShadow;
    float farPlane;
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};  
 * 
 => offset (4 *4 ) + 16 * 4 = 80

struct SunLight {
    int castShadow;
    float intensity;
    float farPlane;
    vec3 direction;
    vec3 diffuse;
    vec3 depth;
};
=> offset 4 * 2 + 4 +4(reserved) + 16 * 3 = 64;



StaticPoints=> start 0  ,  size 16 + 10 * 96,  end 976

DynamicPoints=> start 1024(256*n, n = 4),  size 10 * 96, end 1984

StaticDirs=> start 2048 (256*n, n = 8), size 10 * 80, end 2848

DynamicDirs=> start 3072 (256*n, n = 12), size 10 * 80, end 3872

SunLight => start 

*/

const int P_LIGHT_STD140_SIZE = 96;
const int D_LIGHT_STD140_SIZE = 80;
const int S_LIGHT_STD140_SIZE = 48;

const int STATIC_POINT_LIGHT_NUM_OFFSET = 0;
const int DYNAMIC_POINT_LIGHT_NUM_OFFSET = 4;
const int STATIC_DIR_LIGHT_NUM_OFFSET = 8;
const int DYNAMIC_DIR_LIGHT_NUM_OFFSET = 12;

const int STATIC_POINT_LIGHTS_OFFSET = 16;  // 4 numbers ahead of arrays.

// here because it is inside of one std140,  do not need to calcuate 256*n, stand side by side.
const int DYNAMIC_POINT_LIGHTS_OFFSET = STATIC_POINT_LIGHTS_OFFSET + 96 * 10;

const int STATIC_DIR_LIGHTS_OFFSET = DYNAMIC_POINT_LIGHTS_OFFSET + 96 * 10;
const int DYNAMIC_DIR_LIGHTS_OFFSET = STATIC_DIR_LIGHTS_OFFSET + 80 * 10;

const int SUN_LIGHTS_OFFSET = DYNAMIC_DIR_LIGHTS_OFFSET + 10 * 80;


class LightUniformBufferBindings
{
public:
    inline static UniformBufferBindingSlot LightSlot = {
        (unsigned int)UBOBindings::LIGHTS, 
        0, 
        "Lights", 
        4 * 10 + P_LIGHT_STD140_SIZE * constants::MAX_POINT_LIGHTS * 2 + 
        D_LIGHT_STD140_SIZE * constants::MAX_DIR_LIGHTS * 2 +
        S_LIGHT_STD140_SIZE
        };
    
    static void BindLights(unsigned int shaderId)
    {
        LightUniformBuffer::GetUniformBuffer()->BindShaderSlot(shaderId, LightSlot);
    }

    static void FeedStaticPointLights(ShaderPointLight *lights, int count)
    {
        LightUniformBuffer *ub = LightUniformBuffer::GetUniformBuffer();
        ub->Feed(count, STATIC_POINT_LIGHT_NUM_OFFSET);
        ub->Feed(lights, count, STATIC_POINT_LIGHTS_OFFSET);
    }

    static void FeedDynamicPointLights(ShaderPointLight *lights, int count)
    {
        LightUniformBuffer *ub = LightUniformBuffer::GetUniformBuffer();
        ub->Feed(count, DYNAMIC_POINT_LIGHT_NUM_OFFSET);
        ub->Feed(lights, count, DYNAMIC_POINT_LIGHTS_OFFSET);
    }
    static void FeedSunLight(ShaderSunLight * sun)
    {
        LightUniformBuffer *ub = LightUniformBuffer::GetUniformBuffer();
        ub->Feed(sun, SUN_LIGHTS_OFFSET);
    }

    static void FeedDynamicDirLights(ShaderDirLight * lights, int count)
    {
        LightUniformBuffer *ub = LightUniformBuffer::GetUniformBuffer();
        ub->Feed(count, DYNAMIC_DIR_LIGHT_NUM_OFFSET);
        ub->Feed(lights, count, DYNAMIC_DIR_LIGHTS_OFFSET);
    }
    static void FeedStaticDirLights(ShaderDirLight * lights, int count)
    {
        LightUniformBuffer *ub = LightUniformBuffer::GetUniformBuffer();
        ub->Feed(count, STATIC_DIR_LIGHT_NUM_OFFSET);

        ub->Feed(lights, count, STATIC_DIR_LIGHTS_OFFSET);
    }
};

class SettingsUniformBufferBindings
{
public:
    inline static UniformBufferBindingSlot SettingsSlot = {
        (unsigned int)UBOBindings::SETTINGS, 
        0, 
        "Settings", 
        4 * 16
    };

    static void BindSettings(unsigned int shaderId)
    {
        SettingsUniformBuffer::GetUniformBuffer()->BindShaderSlot(shaderId, SettingsSlot);
    }
    static void FeedSettings(ShaderSettings *settings)
    {
        SettingsUniformBuffer *ub = SettingsUniformBuffer::GetUniformBuffer();
        ub->Feed(settings->hasHDRIMap, 0);
        ub->Feed(settings->useShadowPCF, 4);
        // ...
    }
};

class BoneUniformBufferBindings
{
public:
    inline static UniformBufferBindingSlot BoneSlot = {
        (unsigned int)UBOBindings::BONES,
        0,
        "Bones",
        constants::MAX_BONE_NUM * sizeof(glm::mat4)
    };
    static void BindBones(unsigned int shaderId)
    {
        BoneUniformBuffer::GetUniformBuffer()->BindShaderSlot(shaderId, BoneSlot);
    }

    static void FeedBones(glm::mat4* bones, int boneNum)
    {
        BoneUniformBuffer* ub = BoneUniformBuffer::GetUniformBuffer();
        ub->Feed(bones, boneNum);
    }

};

class UniformBufferBindings
{
public:
    static void BindSlot(unsigned int shaderId, UniformBufferBindingSlot &slot)
    {
        if (LightUniformBufferBindings::LightSlot == slot)
        {
            LightUniformBufferBindings::BindLights(shaderId);
        }
        else if (BoneUniformBufferBindings::BoneSlot == slot) {
            BoneUniformBufferBindings::BindBones(shaderId);
        }
        else if (SettingsUniformBufferBindings::SettingsSlot == slot) {
            SettingsUniformBufferBindings::BindSettings(shaderId);
        } else {
            MatricesUniformBufferBindings::BindSlot(shaderId, slot);
        }
    }
};

