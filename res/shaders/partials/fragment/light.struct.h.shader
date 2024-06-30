struct DirLight {
    int index;
    int castShadow;
    float intensity;
    float farPlane;
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};  
struct SunLight {
    int castShadow;
    float intensity;
    float farPlane;
    int enable;
    vec3 direction;
    vec3 diffuse;
    vec3 depth;
};
struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight {
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