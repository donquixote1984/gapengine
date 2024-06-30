#pragma once
#include <string>
#include <map>
struct ShaderTemplateKeys
{
    
    static inline std::string location1 = "__A_LOCATION_1__";
    static inline std::string location2 = "__A_LOCATION_2__";
    static inline std::string location3 = "__A_LOCATION_3__";
    static inline std::string location4 = "__A_LOCATION_4__";
    static inline std::string location5 = "__A_LOCATION_5__";

    static inline std::string positionMultiplier = "__POSITION_MULTIPLIER__";
    static inline std::string instanceCount = "__U_ATTR_INSTANCINT_COUNT__";
    static inline std::string instancingUV = "__INSTANCING_UV__";
    static inline std::string instancingUVFrag = "__INSTANCING_UV_FRAG__";

    static inline std::string out1 = "__A_OUT_1__";
    static inline std::string out2 = "__A_OUT_2__";
    static inline std::string out3 = "__A_OUT_3__";

    static inline std::string uniformTile = "__U_ATTR_TILE__";
    static inline std::string tilableMultiplier = "__TILE_FACTOR__";

    static inline std::string TBN = "__TBN__";
    static inline std::string OUT_TBN_TESS = "__OUT_TBN_TESS__";
    static inline std::string TBN_TESS = "__TBN_TESS__";
    static inline std::string TBN_frag_in = "__TBN_FRAG_IN__";
    static inline std::string TBN_frag_normal = "__TBN_FRAG_NORMAL__";
    static inline std::string TERRAIN_SHADER = "__TERRAIN_SHADER__";

    static inline std::string SHADOW_VERT_HEADER = "__SHADOW_VERT_HEADER__";
    static inline std::string SHADOW_VERT_MAIN = "__SHADOW_VERT_MAIN__";
    static inline std::string SHADOW_FRAG_HEADER = "__SHADOW_FRAG_HEADER__";
    static inline std::string SHADOW_FRAG_MAIN = "__SHADOW_FRAG_MAIN__";

    static inline std::string ATOMSPHERE_DECLARATIONS = "__ATOMSPHERE_DECLARATIONS__";
    static inline std::string SUN_IRRADIANCE = "__SUN_IRRADIANCE__";
    static inline std::string ATOMSPHERE_TRANSMITTANCE = "__ATOMSPHERE_TRANSMITTANCE__";

    static inline std::string BONE_DEF = "__BONE_DEF__";
    static inline std::string BONE_CALC = "__BONE_CALC__";

    static std::map<std::string, std::string> GetDefault()
    {
        std::map<std::string, std::string> map;
        map[ShaderTemplateKeys::location1]          = "";
        map[ShaderTemplateKeys::location2]          = "";
        map[ShaderTemplateKeys::location3]          = "";
        map[ShaderTemplateKeys::location4]          = "";
        map[ShaderTemplateKeys::out1]               = "";
        map[ShaderTemplateKeys::out2]               = "";
        map[ShaderTemplateKeys::out3]               = "";
        map[ShaderTemplateKeys::uniformTile]        = "";
        map[ShaderTemplateKeys::tilableMultiplier]  = "";
        map[ShaderTemplateKeys::positionMultiplier] = "";
        map[ShaderTemplateKeys::TBN]                = "";
        map[ShaderTemplateKeys::TBN_frag_normal]    = "";
        map[ShaderTemplateKeys::TBN_frag_in]        = "";
        map[ShaderTemplateKeys::SHADOW_FRAG_HEADER] = "";
        map[ShaderTemplateKeys::SHADOW_FRAG_MAIN]   = "";
        map[ShaderTemplateKeys::SHADOW_VERT_HEADER] = "";
        map[ShaderTemplateKeys::SHADOW_VERT_MAIN]   = "";
        map[ShaderTemplateKeys::OUT_TBN_TESS]       = "";
        map[ShaderTemplateKeys::TBN_TESS]           = "";
        map[ShaderTemplateKeys::TERRAIN_SHADER]     = "";
        map[ShaderTemplateKeys::ATOMSPHERE_DECLARATIONS] = "";
        map[ShaderTemplateKeys::SUN_IRRADIANCE]     = "";
        map[ShaderTemplateKeys::ATOMSPHERE_TRANSMITTANCE] = "";
        map[ShaderTemplateKeys::BONE_DEF] = "";
        map[ShaderTemplateKeys::BONE_CALC] = "";
        return map;
    }
};