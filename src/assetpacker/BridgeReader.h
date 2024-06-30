#pragma once
#include <filesystem>
#include <string>
#include <iostream>
#include <nlohmann/json.hpp>
#include <fstream>
#include "AssetFile.h"
#include "Meta.h"
#include "Reader.h"
#include "ReaderException.h"

using json = nlohmann::json;

namespace assetpacker {
    class BridgeReader : public Reader
    {
    private:
        int lod=3;
        std::string m_MetaJsonPath;
        json m_Meta;
        
        bool ModelFilter(const json &model);
        bool TextureFilter(const json &texture);
        void ProcessModels();
        void ProcessTextures();
        int m_TextureRes = 0;
    public:
        
        BridgeReader(const std::string &path);
        void ReadMeta();
        bool Validate() override;
        void Read() override;
        json GetModels(json meta);
        json GetTextures(json meta);
       // char *ToModelBinary( int & size);
        std::string ToString() const;

    };
}