#pragma once

#include <string>
#include <chrono>
#include <ctime>
#include <algorithm>
#include <filesystem>
#include <bitset>
#include <cctype>

namespace assetpacker {

    const int MAX_MODEL_COUNT = 10;
    const int MAX_TEXTURE_CHANNELS = 50;  // support multiple materials
    const int HEADER_SIGN = 20150315;

    const int SEG_MARGIN = 4 * 128;
    const int MODEL_MARGIN = 128;
    const int TEX_MARGIN = 128;
    struct SizeOffsetTypeMime
    {
        unsigned int size; //4
        long offset; // 4
        unsigned int type; //4
        unsigned int mime; //4
        char name[100];  // 200
    };

    struct HeaderStruct
    {
        int header = HEADER_SIGN;   //4
        int mcount = 0;                 //4
        int tcount = 0;                 //4
        SizeOffsetTypeMime modelSizeAndOffset[MAX_MODEL_COUNT]; // 216*10
        SizeOffsetTypeMime textureSizeAndOffset[MAX_TEXTURE_CHANNELS]; //216 * 50
        int margin = 0; // 4
    };  // size: 


    enum class TextureType
    {
        COLOR,
        NORMAL,
        ROUGHNESS,
        METALNESS,
        OPACITY,
        SPECULAR,
        UNSUPPORTED
    };

    enum class ModelType
    {
        OBJ,
        FBX,
        UNSUPPORTED,
    };
    enum class MimeType
    {
        JPG,
        PNG,
        EXR,
        UNSUPPORTED
    };

    struct Model {
        long size = 0;
        std::string uri = "";
        ModelType type = ModelType::FBX;
    };


    struct Texture {
        long size = 0;
        std::string uri = "";
        TextureType type = TextureType::UNSUPPORTED;
        MimeType mineType = MimeType::JPG;
    };

    MimeType GetMimeTypeByPath(const std::string& path);
    MimeType GetMimeTypeByStr(const std::string& mimeType);
    TextureType GetTypeByStr(std::string type);
    bool IsAvailableType(std::string type);
    std::string GetTimeMills();
    std::string GetMimeSuffix(unsigned int mt);
    std::string GetModelSuffix(unsigned int mt);

    TextureType GetTexType(unsigned int tt);
    Texture GetTexStruct(const std::string& path);
    ModelType GetModelTypeByPath(const std::string& path);
    Model GetModelStruct(const std::string& path);

    std::string GetSampleTextureName(TextureType tt);

    bool IsColorTex(const std::string path);
    bool IsNormalTex(const std::string path);
    bool IsSpecularTex(const std::string path);
    bool IsMetalnessTex(const std::string path);
    bool IsOpacityTex(const std::string path);
    bool IsRoughnessTex(const std::string path);
    bool isTexture(const std::string path);
    bool isModel(const std::string path);

    bool StrEndsWith(std::string const& value, std::string const& ending);
    bool StrEndsWithIgnoreCase(std::string const& value, std::string const& ending);

    std::string ToLowerStr(const std::string str);

}