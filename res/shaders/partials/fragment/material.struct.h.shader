struct Channel {
    vec3 color;
    sampler2D tex;
    int hasTexture;
    int enable;
    bool type;
    float constant;
    int reverseY;
};

struct ConstantChannel
{
    vec3 color;
    int enable;
    float constant;
};
struct Material {
    Channel ambient;
    Channel diffuse;
    Channel specular;
    Channel normal;
    Channel roughness;
    Channel metalness;
    ConstantChannel shininess;
};

struct Water {
    Channel tint;
    Channel transparency;
    Channel flow;
    Channel normal;
    ConstantChannel waveStrength;
    ConstantChannel waveSpeed;
};