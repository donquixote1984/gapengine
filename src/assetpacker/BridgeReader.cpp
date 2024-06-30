#include "BridgeReader.h"
namespace assetpacker {
    BridgeReader::BridgeReader(const std::string &path): Reader(path)
    {}

    bool BridgeReader::Validate()
    {
        if(!Reader::Validate())
        {
            return false;
        }
       
        for(const auto f : std::filesystem::directory_iterator(m_Dir))
        {
            std::filesystem::path filepath = f.path();
            if (filepath.extension() == ".json")
            {
                m_MetaJsonPath = filepath.string();
                return true;
            }
        }
        std::cout << "No json file in Directory: " + m_Dir << std::endl;
        return false;
    }

    void BridgeReader::ReadMeta()
    {
        std::ifstream f(m_MetaJsonPath);
        m_Meta = json::parse(f);

        if(!m_Meta.is_object())
        {
            std::cout << "Invalid Json Format in " + m_MetaJsonPath << std::endl;
            return;
        }
    }
    json BridgeReader::GetModels(json meta)
    {
        json models;

        if (meta.contains("models") || meta.contains("meshes"))
        {
            if (meta.contains("models")) 
            {
                models = meta["models"];
            }
            else if (meta.contains("meshes"))
            {
                for (const auto & mesh : meta["meshes"])
                {
                    if (mesh.contains("uris"))
                    {
                        for (const auto &uri: mesh["uris"])
                        {
                            models.push_back(uri);
                        }
                    }
                }
            }
        } else 
        {
            //throw ReaderException("no 'models' or 'meshes' in metadata");
        }
        return models;
    }
    void BridgeReader::ProcessModels()
    {
        json models = GetModels(m_Meta);
        
        for (const auto &model: models)
        {
            if (ModelFilter(model))
            {
                int contentLength = model["contentLength"];
                std::string ruri = model["uri"];
                //std::ifstream i;
                //i.open(m_Path + "/" + uri, std::ios::in | std::ios::binary);
                //m_AssetFile->Append(i, contentLength);
                std::string uri = m_Dir + "/" + ruri;
                m_Models[m_ModelCount] = {contentLength, uri, ModelType::FBX};
                m_ModelCount += 1;
                if (m_ModelCount > MAX_MODEL_COUNT)
                {
                    return;
                }
            }
        }
    }

    json BridgeReader::GetTextures(json meta)
    {
        json textures;
        if (meta.contains("maps") || meta.contains("components"))
        {
            if (meta.contains("maps"))
            {
                textures = meta["maps"];
            }

            else if (meta.contains("components"))
            {
                for (const auto &comp : meta["components"])
                {
                    std::string type = comp["type"];
                    for (const auto &uri : comp["uris"])
                    {
                        for (const auto &res : uri["resolutions"])
                        {
                            std::string resolution = res["resolution"];
                            for (const auto &format: res["formats"])
                            {
                                std::string ruri = format["uri"];
                                std::string uri = m_Dir + "/" + ruri;
                                if (filesystem::exists(uri))
                                {
                                    json singleTexture;
                                    singleTexture["type"] = type;
                                    singleTexture["mimeType"] = format["mimeType"];
                                    singleTexture["contentLength"] = format["contentLength"];
                                    singleTexture["uri"] = ruri;
                                    singleTexture["resolution"] = resolution;
                                    textures.push_back(singleTexture);
                                }
                            }
                        }
                    }
                }
            }
        }
        else {
            throw ReaderException("no 'maps' or 'components' in meta for textures");
        }

        return textures;
    }
    void BridgeReader::ProcessTextures()
    {
    json textures = GetTextures(m_Meta);
    for (const auto &texture: textures)
    {
        if (TextureFilter(texture))
        {
            int contentLength = texture["contentLength"];
            std::string ruri = texture["uri"];
            std::string uri = m_Dir + "/" + ruri;
            TextureType type = GetTypeByStr(texture["type"]);
            MimeType mtype = GetMimeTypeByStr(texture["mimeType"]);

            m_Textures[m_TextureCount] = {contentLength, uri, type, mtype};
            m_TextureCount += 1;

            if (m_TextureCount > MAX_TEXTURE_CHANNELS)
            {
                return;
            }
        }
    }
    }

    void BridgeReader::Read()
    {
        if(!Validate()) {
            return;
        }
        ReadMeta();
        ProcessModels();
        ProcessTextures();
    }

    bool BridgeReader::ModelFilter(const json &model)
    {
        std::string mt = model["mimeType"];
        
        if ( mt == "application/x-fbx" )
        {
            if (model.contains("lod"))
            {
                int lod = model["lod"];
                if (lod == 3)
                {
                    return true;
                }
            } else {
                std::string uri = model["uri"];
                if (StrEndsWithIgnoreCase(uri, "_LOD3.fbx"))
                {
                    return true;
                }
            }
        }

        return false;
    }

    bool BridgeReader::TextureFilter(const json &texture)
    {
        std::string res = texture["resolution"];
        std::string uri = texture["uri"];
        std::string mimeType = texture["mimeType"];
        std::string testPath = (m_Dir + "/" + uri);
        std::string type = texture["type"];

        if (!IsAvailableType(type))
        {
            return false;
        }

        if (mimeType == "image/jpeg")
        {
            if(m_TextureRes == 0 && std::filesystem::exists(testPath)) {
                m_TextureRes = (int)(res[0] - '0');
                return true;
            }

            if (m_TextureRes == (int)(res[0] - '0') && std::filesystem::exists(testPath))
            {
                return true;
            }
        }

        return false;
    }

    std::string BridgeReader::ToString() const 
    {
        std::string output;

        for (auto & model: m_Textures)
        {
            output += ("size: " + std::to_string((int)model.size) + ", uri: " + model.uri + "\n");
        };
        return output;
    }

}