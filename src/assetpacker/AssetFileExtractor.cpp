#include "AssetFileExtractor.h"

namespace assetpacker {
    AssetFileExtractor::AssetFileExtractor(const std::string& outputPath)
    {
        std::srand(unsigned(std::time(nullptr)));
        m_OutputDir = outputPath;
        if (!m_OutputDir.empty() && !StrEndsWith(m_OutputDir, "/"))
        {
            m_OutputDir += "/";
        }
    }
    bool AssetFileExtractor::Extract(const std::string& inputfilepath)
    {
        // temp/asset/models/grass/grass1/model/xxxx.model.fbx => 
        // . m_Filename = asset/models/grass/grass1.asset
        //  filepath = temp/asset/models/grass/grass1
        // 

        m_Filename = inputfilepath;

        if (!filesystem::exists(m_Filename)) {
            std::cout << "File: " + m_Filename + " does not exist.";
            return false;
        }
        if (!ExtractHeader())
        {
            return false;
        }

        ExtractModels();
        ExtractTextures();

        return true;
    }

    bool AssetFileExtractor::ExtractHeader()
    {
        ifstream input;
        input.open(m_Filename, std::ios::in | std::ios::binary);
        input.read((char*)&m_HeaderStruct, sizeof(HeaderStruct));
        if (m_HeaderStruct.header != HEADER_SIGN)
        {
            std::cout << "Invalid Asset file header" << std::endl;
            input.close();
            return false;
        }
        input.close();
        //std::cout << ((int)t.mcount) << std::endl;
        //return header;
        //ReadHeader(input);
        return true;
    }



    bool AssetFileExtractor::ExtractModels()
    {
        ifstream input;
        input.open(m_Filename, std::ios::in | std::ios::binary);

        if (!filesystem::exists(m_OutputDir + MODEL_FOLDER_PREFIX))
        {
            std::filesystem::create_directories(m_OutputDir + MODEL_FOLDER_PREFIX);
        }

        for (int i = 0; i < min(m_HeaderStruct.mcount, MAX_MODEL_COUNT); i++)
        {

            SizeOffsetTypeMime mblock = m_HeaderStruct.modelSizeAndOffset[i];
            unsigned int offset = mblock.offset;
            unsigned int size = mblock.size;
            unsigned int type = mblock.type;

            std::string basename = GetTimeMills();
            std::string filepath;

            filepath = m_OutputDir + MODEL_FOLDER_PREFIX + basename + ".model" + GetModelSuffix(type);
            fstream outfile;
            outfile.open(filepath, std::ios::out | std::ios::binary);
            input.seekg(offset);
            char* content = new char[size];
            input.read(content, size);
            outfile.write(content, size);
            outfile.flush();
            outfile.close();
            delete[] content;
            /*
            std::bitset<128> mblock = m_HeaderStruct.modelSizeAndStartBit[i];
            std::bitset<128> bs(INT32_MAX);

            int type = (int)(mblock & bs).to_ullong();
            int offset = (int)((mblock >> 32) & bs).to_ullong();
            int size = (int)((mblock >> 64)).to_ullong();

            std::string basename = GetTimeMills();
            std::string filepath;

            filepath = m_OutputDir + MODEL_FOLDER_PREFIX + basename + ".model"+ GetModelSuffix(type);
            fstream outfile;
            outfile.open(filepath, std::ios::out | std::ios::binary);
            input.seekg(offset);
            char *content = new char[size];
            input.read(content, size);
            outfile.write(content, size);
            outfile.flush();
            outfile.close();
            delete [] content;
            */
        }
        input.close();
        return true;
    }

    bool AssetFileExtractor::ExtractTextures()
    {
        ifstream input;
        input.open(m_Filename, std::ios::in | std::ios::binary);
        std::string basename = GetTimeMills();

        if (!filesystem::exists(m_OutputDir + TEX_FOLDER_PREFIX))
        {
            std::filesystem::create_directories(m_OutputDir + TEX_FOLDER_PREFIX);
        }
        for (int i = 0; i < min(m_HeaderStruct.tcount, MAX_TEXTURE_CHANNELS); i++)
        {
            SizeOffsetTypeMime tblock = m_HeaderStruct.textureSizeAndOffset[i];
            int mime = (int)tblock.mime;
            int type = (int)tblock.type;
            int offset = (int)tblock.offset;
            int size = (int)tblock.size;
            //char * name = tblock.name;
            std::string name = tblock.name;
            TextureType tt = GetTexType(type);

            std::string outputPath = "";
            if (name.empty())
            {
                outputPath = m_OutputDir + TEX_FOLDER_PREFIX + basename + GetSampleTextureName(tt) + GetMimeSuffix(mime);
            }
            else {
                outputPath = m_OutputDir + TEX_FOLDER_PREFIX + name;
            }
            ofstream outfile;
            outfile.open(outputPath, std::ios::out | std::ios::binary);
            input.seekg(offset);
            char* content = new char[size];
            input.read(content, size);
            outfile.write(content, size);
            outfile.flush();
            outfile.close();
            delete[] content;


            /*
            std::bitset<128> tblock = m_HeaderStruct.textureSizeAndStartBit[i];
            int mime   =(int) (tblock & bs16).to_ullong(); // low 16 bit
            int type   = (int)(tblock >> 16 & bs16).to_ullong(); // medium 16 bit
            int offset = (int)(tblock >> 32 & bs32).to_ullong(); // high 32 bit
            int size   = (int)(tblock >> 64).to_ullong() ; // high high 32bit
            TextureType tt = GetTexType(type);

            std::string outputPath = m_OutputDir + TEX_FOLDER_PREFIX + basename + GetSampleTextureName(tt) + GetMimeSuffix(mime);
            ofstream outfile;
            outfile.open(outputPath, std::ios::out | std::ios::binary);
            input.seekg(offset);
            char *content = new char[size];
            input.read(content, size);
            outfile.write(content, size);
            outfile.flush();
            outfile.close();
            delete [] content;
            */
        }
        return true;
    }

}