#pragma once
#include <iostream>
#include <chrono>
#include <ctime>
#include <fstream>
#include <vector>
#include "Meta.h"
#include "AssetFileExtractor.h"
#include <bitset>

using namespace std;
namespace assetpacker {
    class AssetFile
    {
    private:
        std::string m_Filename;
        std::vector<Model> m_Models;
        std::vector<Texture> m_Textures;
        int margin = sizeof(int);
    public:
        void CreateFile();
        void AppendHeader(char * startptr);
        void Append(ifstream &input, int contentSize);
        void AppendModel(Model *model, int countv);
        void AppendTexture(Texture *model, int count);
        bool Created();
        void AddModelMeta();
        void AddTextureMeta();
        void ReadHeader(ifstream &input);
        std::string GetFilePath();

        const void operator >> (AssetFileExtractor *afe);
    };
};

