#pragma once
#include <glm/glm.hpp>
/***
 * 
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
*/
struct ShaderSettings
{
    int hasHDRIMap;
    int useShadowPCF;
};

struct ShaderPointLight
{
    int index;
    float intensity;
    float constant;
    float linear;
    float quadratic;
    int castShadow;
    float farPlane;
    float bak;
    glm::vec4 position;
    glm::vec4 ambient;
    glm::vec4 diffuse;
    glm::vec4 specular;
};

/***
 * 
 * struct DirLight {
    int index;
    int castShadow;
    float intensity;
    float farPlane;
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};  

*/
struct ShaderDirLight
{
    int index;
    int castShadow;
    float intensity;
    float farPlane;
    
    glm::vec4 direction;
    glm::vec4 ambient;
    glm::vec4 diffuse;
    glm::vec4 specular;
};

struct ShaderSunLight
{
    int castShadow;
    float intensity;
    float farPlane;
    int enable = 0;
    glm::vec4 direction;
    glm::vec4 diffuse;
    glm::vec4 depth; // camera position - earth center
};