#pragma once
#include <unordered_map>
#include <fmt/format.h>
#include "ShaderTemplate.h"
#include "Shader.h"

class ShaderSnippet
{
private:
    std::unordered_map<std::string, std::string> snippet;
public:
    void AddArgs(std::string key, std::string value);
    void ApplyToShader(Shader * shader);
    
    static ShaderSnippet InstancingSnippet(int slot)
    {
        //std::unordered_map<std::string, std::string> snippet;
        ShaderTemplateKeys keys;
        std::string shaderStr = fmt::format("layout (location = {}) in mat4 instanceMatrix;", slot);
        //snippet[keys.location1] = shaderStr;
        //snippet[keys.positionMultiplier] = " * instanceMatrix ";
        ShaderSnippet ss;
        ss.AddArgs(keys.location1, shaderStr);
        ss.AddArgs(keys.positionMultiplier, " * instanceMatrix ");
        ss.AddArgs(keys.instanceCount, "uniform int instancingCount;");
        ss.AddArgs(keys.instancingUV, "instancingUV = float(gl_InstanceID) / float(instancingCount);");
        ss.AddArgs(keys.instancingUVFrag, "in float instancingUV;");
        ss.AddArgs(keys.out3, "out float instancingUV;");
        
        return ss;
    }

    static ShaderSnippet TilableSnippet(const std::string &uniform)
    {
        
        std::string uniformStr= fmt::format("uniform float {};", uniform);
        ShaderSnippet ss;
        ss.AddArgs(ShaderTemplateKeys::uniformTile, uniformStr);
        std::string shaderStr= fmt::format(" * {}", uniform);
        ss.AddArgs(ShaderTemplateKeys::tilableMultiplier, shaderStr);
        return ss;
    }

    static ShaderSnippet TangentSnippet(int tslot, int bslot, bool tess = false)
    {
        std::string tangentLayout   = fmt::format("layout (location = {}) in vec3 tangent;", tslot);
        std::string bitangentLayout = fmt::format("layout (location = {}) in vec3 bitangent;", bslot);
        std::string outTBN = "out mat3 TBN;";
        std::string tbn = R"(
            vec3 T = normalize(normalMatrix * tangent);
            vec3 B = normalize(normalMatrix * bitangent);
            TBN = mat3(T, B, Normal);
        )";

        std::string outTbnTess = R"(
            out mat3 TBN;
        )";

        std::string tbnTess = R"(
            vec3 tangent, bitangent;
            getTangent(p, puv1, pu1v1, pu1v, texCoord, texCoorduv1, texCoordu1v1, texCoordu1v, tangent, bitangent);
            TBN = mat3(tangent, bitangent, Normal); 
        )";
        std::string tbnFragIn = "in mat3 TBN;";
        std::string tbnFragNormal = "norm = normalize(TBN * norm);";
        ShaderSnippet ss;

        ss.AddArgs(ShaderTemplateKeys::out1,      outTBN);
        ss.AddArgs(ShaderTemplateKeys::OUT_TBN_TESS, outTbnTess);
        ss.AddArgs(ShaderTemplateKeys::TBN_TESS, tbnTess);

        ss.AddArgs(ShaderTemplateKeys::location2, tangentLayout);
        ss.AddArgs(ShaderTemplateKeys::location3, bitangentLayout);
        ss.AddArgs(ShaderTemplateKeys::TBN, tbn);
        ss.AddArgs(ShaderTemplateKeys::TBN_frag_in, tbnFragIn);
        ss.AddArgs(ShaderTemplateKeys::TBN_frag_normal, tbnFragNormal);
        return ss;
    }

    static ShaderSnippet TerrainSnippet(bool gpu)
    {
         ShaderSnippet ss;
         std::string includeStr = "";
         if (gpu) {
            includeStr =  R"(
                #include 'geometry.terrain.gpu.shader'
            )";
         } else {
            includeStr =  R"(
                #include 'geometry.terrain.cpu.shader'
            )";
         }
         ss.AddArgs(ShaderTemplateKeys::TERRAIN_SHADER, includeStr);
         return ss;
    }

    static ShaderSnippet AnimationSnippet()
    {
        ShaderSnippet ss;
        std::string bone = "layout (location = 5) in vec4 boneIds;";
        std::string weight = "layout (location = 6) in vec4 weights;";
        ss.AddArgs(ShaderTemplateKeys::location4, bone);
        ss.AddArgs(ShaderTemplateKeys::location5, weight);
        std::string boneDef = R"(
            int MAX_BONES = 600;
            const int MAX_BONE_INFLUENCE = 4;
        )";

        ss.AddArgs(ShaderTemplateKeys::BONE_DEF, boneDef);


        std::string boneCalc = R"(
            if (u_IsPlaying == 1) 
            {
                vec4 totalPosition = vec4(0.0f);
                vec3 totalNormal = vec3(0.0f);
                for(int i = 0 ; i < MAX_BONE_INFLUENCE ; i++)
                {
                    if(boneIds[i] < 0.0) 
                        continue;
                    if(boneIds[i] >=MAX_BONES) 
                    {
                        totalPosition = pos;
                        break;
                    }
                    vec4 localPosition = bonesMatrices[int(boneIds[i])] * pos;
                    totalPosition += localPosition * weights[i];
                
                    vec3 localNormal = mat3(bonesMatrices[int(boneIds[i])]) * norm;
                    totalNormal+=localNormal * weights[i];
                }
                pos = totalPosition;
                norm = totalNormal;
            }
           
        )";
        ss.AddArgs(ShaderTemplateKeys::BONE_CALC, boneCalc);
        return ss;
    }

    static ShaderSnippet AtomsphereSnippet()
    {
        ShaderSnippet ss;
        std::string atomsphere = R"(
            vec3 GetSolarRadiance();
            vec3 GetSkyRadiance(vec3 camera, vec3 view_ray, float shadow_length,
            vec3 sun_direction, out vec3 transmittance);
            vec3 GetSkyRadianceToPoint(vec3 camera, vec3 point, float shadow_length,
            vec3 sun_direction, out vec3 transmittance);
            vec3 GetSunAndSkyIrradiance(
            vec3 p, vec3 normal, vec3 sun_direction, out vec3 sky_irradiance);
        )";

        std::string sunIrradiance = R"(
            sun_irradiance = GetSunAndSkyIrradiance(light.depth, norm, L, sky_irradiance);
        )";
        std::string atomsphereTransmittance = R"(
            GetSkyRadianceToPoint(ub_SunLight.depth, FragPos - earth_center, 0, -ub_SunLight.direction, transmittance); 
        )";

        ss.AddArgs(ShaderTemplateKeys::ATOMSPHERE_DECLARATIONS, atomsphere);
        ss.AddArgs(ShaderTemplateKeys::SUN_IRRADIANCE, sunIrradiance);
        ss.AddArgs(ShaderTemplateKeys::ATOMSPHERE_TRANSMITTANCE, atomsphereTransmittance);
        return ss;
    }

    static ShaderSnippet ShadowSnippet()
    {
        ShaderSnippet ss;
        std::string shadowVertHeader = R"(
            uniform int hasShadowMap;
            uniform mat4 lightSpaceMatrix;
        )";

        ss.AddArgs(ShaderTemplateKeys::SHADOW_VERT_HEADER, shadowVertHeader);

        std::string shadowVertMain = R"(
        )";

        ss.AddArgs(ShaderTemplateKeys::SHADOW_VERT_MAIN, shadowVertMain);

        std::string shadowFragMain = R"(
        )";

        ss.AddArgs(ShaderTemplateKeys::SHADOW_FRAG_MAIN, shadowFragMain);
        return ss;
    }
};