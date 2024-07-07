#pragma once
#include <string>
#include <fstream>
#include <iostream>
#include <bitset>
#include "Meta.h"
#include "Config.h"
#include <limits.h>

using namespace std;
namespace assetpacker {
    class AssetFileExtractor
    {
    private:
        std::string m_Filename;
        std::string m_OutputDir;
        std::string timestamp;
        HeaderStruct m_HeaderStruct;

        bool ExtractHeader();

    public:
        AssetFileExtractor(const std::string& outputPath = "");
        bool Extract(const std::string& filepath);

        bool ExtractModels();
        //bool ExtractModels(const std::string &inputfilepath);

        bool ExtractTextures();
        //bool ExtractTextures(const std::string &filepath, std::string &basename);
    };
}