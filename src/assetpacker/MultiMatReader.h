#pragma once
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/Importer.hpp>
#include <filesystem>
#include <unordered_map>
#include <fmt/format.h>
#include "Reader.h"
#include "Meta.h"

using namespace std;
namespace assetpacker {
    class MultiMatReader : public Reader
    {
    private:
        bool ValidateModel();
        bool ValidateTextures();
        void ValidateTextureChannel(aiMaterial* mat, aiTextureType t);
        void ReadFolderTextures();
        void AddTextures(Texture t);
        void ProcessNode(const aiScene* scene);
        std::string m_ModelPath = "";
        std::unordered_map<std::string, Texture> m_FolderTextures;
    public:
        MultiMatReader(std::string path);
        bool Validate() override;
        void Read() override;
        virtual char* ToHeaderBinary(int& size) override;
    };
}