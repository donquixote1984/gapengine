#pragma once

#include "BasicMaterial.h"
#include <unordered_map>
#include "../textures/Texture.h"
#include "MaterialAttrib.h"


/**
 * 
 *     LIPT_AMBIENT,
    LIPT_DIFFUSE,
    LIPT_CONSTANT,
    LIPT_LINEAR,
    LIPT_QUADRATIC,
    LIPT_DIRECTION,
    LIPT_SPECUAR
*/
class LightMaterial: public Material
{
protected:
   
    //void BindShaderAttrib(MaterialAttrib mAttrib, unsigned int index) const;
public:
    MaterialAttrib ambient = glm::vec3(.1f);
    MaterialAttrib diffuse = glm::vec3(.5f);
    MaterialAttrib specular = glm::vec3(.5f);
    MaterialAttrib intensity = 1.0f;
    // point light
    MaterialAttrib linear = .5f;
    MaterialAttrib constant = .5f;
    MaterialAttrib quadratic =.5f;
    

    LightMaterial();

    glm::vec3 GetAmbient();
    glm::vec3 GetSpecular();
    glm::vec3 GetDiffuse();
    float GetIntensity();
    float GetLinear();
    float GetConstant();
    float GetQuadratic();

    void InitAttibs();
    void FlushToShader() override;
    //void MVPToShader(glm::mat4 model, glm::mat4 view, glm::mat4 projection, glm::vec3 camPos) const override;
};