#include "Meta.h"
#include <sstream>

using namespace std;
namespace assetpacker {

    MimeType GetMimeTypeByPath(const std::string& path)
    {
        std::string ext = filesystem::path(path).extension().string();

        if (ext == ".png")
        {
            return MimeType::PNG;
        }

        if (ext == ".jpg")
        {
            return MimeType::JPG;
        }

        if (ext == ".exr")
        {
            return MimeType::EXR;
        }

        return MimeType::UNSUPPORTED;
    }
    MimeType GetMimeTypeByStr(const std::string& mimeType)
    {
        if (mimeType == "image/jpeg")
        {
            return MimeType::JPG;
        }

        if (mimeType == "image/jpg")
        {
            return MimeType::JPG;
        }

        if (mimeType == "image/png")
        {
            return MimeType::PNG;
        }

        if (mimeType == "image/x-exr")
        {
            return MimeType::EXR;
        }
        return MimeType::UNSUPPORTED;
    }

    TextureType GetTypeByStr(std::string typeRaw)
    {
        std::string type = ToLowerStr(typeRaw);
        if (type == "albedo")
        {
            return TextureType::COLOR;
        }

        if (type == "roughness")
        {
            return TextureType::ROUGHNESS;
        }

        if (type == "opacity")
        {
            return TextureType::OPACITY;
        }

        if (type == "normal")
        {
            return TextureType::NORMAL;
        }

        if (type == "metalness")
        {
            return TextureType::METALNESS;
        }
        if (type == "specular")
        {
            return TextureType::SPECULAR;
        }

        return TextureType::UNSUPPORTED;
    }

    bool IsAvailableType(std::string type)
    {
        if (GetTypeByStr(type) == TextureType::UNSUPPORTED)
        {
            return false;
        }
        return true;
    }
    std::string getLeftPaddingString(std::string const& str, int n, char paddedChar = ' ')
    {
        std::ostringstream ss;
        ss << std::right << std::setfill(paddedChar) << std::setw(n) << str;
        return ss.str();
    }
    std::string GetTimeMills()
    {
        auto start = std::chrono::system_clock::now();
        std::time_t now = std::chrono::system_clock::to_time_t(start);
        std::tm* ptm = std::localtime(&now);
        char timebuffer[100];
        unsigned short seed = std::rand();
        std::strftime(timebuffer, 20, "%Y%m%d%H%M%S", ptm);
        std::string name = timebuffer;
        name = name + getLeftPaddingString(std::to_string(seed), 6, '0');
        return name;
    }


    std::string GetMimeSuffix(unsigned int mt)
    {
        if (mt == (unsigned int)MimeType::JPG)
        {
            return ".jpg";
        }

        if (mt == (unsigned int)MimeType::PNG)
        {
            return ".png";
        }
        if (mt == (unsigned int)MimeType::EXR)
        {
            return ".exr";
        }
        return "";
    }
    std::string GetModelSuffix(unsigned int mt)
    {
        if (mt == (unsigned int)ModelType::FBX)
        {
            return ".fbx";
        }

        if (mt == (unsigned int)ModelType::OBJ)
        {
            return ".obj";
        }
        return "";
    }

    TextureType GetTexType(unsigned int tt)
    {
        if (tt == (unsigned int)TextureType::COLOR)
        {
            return TextureType::COLOR;
        }

        if (tt == (unsigned int)TextureType::METALNESS)
        {
            return TextureType::METALNESS;
        }

        if (tt == (unsigned int)TextureType::NORMAL)
        {
            return TextureType::NORMAL;
        }

        if (tt == (unsigned int)TextureType::OPACITY)
        {
            return TextureType::OPACITY;
        }
        if (tt == (unsigned int)TextureType::ROUGHNESS)
        {
            return TextureType::ROUGHNESS;
        }

        if (tt == (unsigned int)TextureType::SPECULAR)
        {
            return TextureType::SPECULAR;
        }
        return TextureType::UNSUPPORTED;
    }
    std::string GetSampleTextureName(TextureType tt)
    {
        if (tt == TextureType::COLOR)
        {
            return ".color";
        }
        if (tt == TextureType::METALNESS)
        {
            return ".metalness";
        }

        if (tt == TextureType::NORMAL)
        {
            return ".normal";
        }

        if (tt == TextureType::OPACITY)
        {
            return ".opacity";
        }

        if (tt == TextureType::ROUGHNESS)
        {
            return ".roughness";
        }

        if (tt == TextureType::SPECULAR)
        {
            return ".specular";
        }

        return ".unknown";
    }

    bool IsColorTex(const std::string path)
    {
        if (ToLowerStr(path).find("albedo") != std::string::npos)
        {
            return true;
        }
        if (ToLowerStr(path).find("diffuse") != std::string::npos)
        {
            return true;
        }
        if (ToLowerStr(path).find("basecolor") != std::string::npos)
        {
            return true;
        }
        return false;
    }

    bool IsNormalTex(const std::string path)
    {
        if (ToLowerStr(path).find("normal") != std::string::npos)
        {
            return true;
        }
        return false;
    }
    bool IsSpecularTex(const std::string path)
    {
        if (ToLowerStr(path).find("spec") != std::string::npos)
        {
            return true;
        }
        return false;
    }
    bool IsMetalnessTex(const std::string path)
    {
        if (ToLowerStr(path).find("metallic") != std::string::npos)
        {
            return true;
        }
        if (ToLowerStr(path).find("metalness") != std::string::npos)
        {
            return true;
        }
        return false;
    }
    bool IsOpacityTex(const std::string path)
    {
        if (ToLowerStr(path).find("opacity") != std::string::npos)
        {
            return true;
        }
        if (ToLowerStr(path).find("translucency") != std::string::npos)
        {
            return true;
        }
        return false;
    }
    bool IsRoughnessTex(const std::string path)
    {
        if (ToLowerStr(path).find("rough") != std::string::npos)
        {
            return true;
        }
        return false;
    }

    bool isTexture(const std::string path)
    {
        const std::string testpath = ToLowerStr(path);
        if (StrEndsWith(testpath, ".png"))
        {
            return true;
        }

        if (StrEndsWith(testpath, ".jpg"))
        {
            return true;
        }

        if (StrEndsWith(testpath, ".exr"))
        {
            return true;
        }
        return false;
    }

    bool isModel(const std::string path)
    {
        std::string testpath = ToLowerStr(path);
        if (StrEndsWith(testpath, ".obj"))
        {
            return true;
        }

        if (StrEndsWith(testpath, ".fbx"))
        {
            return true;
        }
        return false;
    }


    std::string ToLowerStr(std::string str) {
        std::transform(str.begin(), str.end(), str.begin(), ::tolower);
        return str;
    }



    bool StrEndsWith(std::string const& value, std::string const& ending)
    {
        if (ending.size() > value.size()) return false;
        return std::equal(ending.rbegin(), ending.rend(), value.rbegin());
    }
    bool StrEndsWithIgnoreCase(std::string const& value, std::string const& ending)
    {
        if (ending.size() > value.size()) return false;
        std::string lowerCaseValue = ToLowerStr(value);
        std::string lowerEnding = ToLowerStr(ending);
        return std::equal(lowerEnding.rbegin(), lowerEnding.rend(), lowerCaseValue.rbegin());
    }

    ModelType GetModelTypeByPath(const std::string& path)
    {
        std::string ext = filesystem::path(path).extension().string();
        if (ext == ".fbx")
        {
            return ModelType::FBX;
        }

        if (ext == ".obj")
        {
            return ModelType::OBJ;
        }

        return ModelType::UNSUPPORTED;
    }

    Model GetModelStruct(const std::string& path)
    {
        Model m;
        m.size = filesystem::file_size(path);
        m.type = GetModelTypeByPath(path);
        m.uri = path;
        return m;
    }
    Texture GetTexStruct(const std::string& path)
    {
        Texture t;
        t.size = filesystem::file_size(path);
        t.mineType = GetMimeTypeByPath(path);
        filesystem::path p = path;
        std::string filename = p.filename().string();
        if (IsNormalTex(filename))
        {
            t.type = TextureType::NORMAL;
        }

        if (IsOpacityTex(filename))
        {
            t.type = TextureType::OPACITY;
        }

        if (IsMetalnessTex(filename))
        {
            t.type = TextureType::METALNESS;
        }

        if (IsRoughnessTex(filename))
        {
            t.type = TextureType::ROUGHNESS;
        }

        if (IsSpecularTex(filename))
        {
            t.type = TextureType::SPECULAR;
        }

        if (IsColorTex(filename))
        {
            t.type = TextureType::COLOR;
        }
        t.uri = path;
        return t;
    }


}