#include "Reader.h"

namespace assetpacker {
    Reader::Reader(const std::string path): m_Dir(path)
    {
        if(path.at(path.size()-1) == '/')
        {
            m_Dir = m_Dir.substr(0, m_Dir.size()-1);
        }
    }

    bool Reader::Validate()
    {
        if(!std::filesystem::exists(m_Dir) || std::filesystem::is_empty(m_Dir))
        {
            std::cout << "Directory : " << m_Dir << " does not exist or empty!"<<std::endl;
            return false;
        }
        return true;
    }

    void Reader::operator>> (AssetFile * assetFile)
    {
        if (!assetFile->Created())
        {
            assetFile->CreateFile();
        }

        m_AssetFile = assetFile;
        Read();

        int size = 0;
        char * startptr = ToHeaderBinary(size);
        //append header
        m_AssetFile->AppendHeader(startptr);
        m_AssetFile->AppendModel(m_Models, m_ModelCount);
        m_AssetFile->AppendTexture(m_Textures, m_TextureCount);
        delete startptr;
        // 
    };

    char * Reader::ToHeaderBinary(int &size)
    {
        //   | - mcount |  m1 size + startbit|  m2 size + startbit|...| m10 size | color texture size + type[ type at low 4bit] | normal texture size | rough texture size | metal texturesize | 
        //
        //

        HeaderStruct *t =  new HeaderStruct();
        
        t->mcount = min(m_ModelCount,MAX_MODEL_COUNT); // 1st   32
        t->tcount = min(m_TextureCount, MAX_TEXTURE_CHANNELS);

        SizeOffsetTypeMime modelSizeAndOffset[MAX_MODEL_COUNT];
        SizeOffsetTypeMime textureSizeAndOffset[MAX_TEXTURE_CHANNELS];


        int modelStartOffset = sizeof(HeaderStruct);

        for (int i = 0 ; i < m_ModelCount; i++)
        {
            //sizesAndStartBit[i] = m_Models[i].size;
            t->modelSizeAndOffset[i].size = m_Models[i].size;
            t->modelSizeAndOffset[i].offset = modelStartOffset;
            t->modelSizeAndOffset[i].type = (unsigned int)m_Models[i].type;
            modelStartOffset += m_Models[i].size;
        }

        int textureStartOffset = modelStartOffset;

        for (int i = 0; i < m_TextureCount; i++)
        {
            auto type = (unsigned int)m_Textures[i].type;
            auto mime = (unsigned int)m_Textures[i].mineType;

            t->textureSizeAndOffset[i].type = type;
            t->textureSizeAndOffset[i].mime = mime;
            t->textureSizeAndOffset[i].size = m_Textures[i].size;
            t->textureSizeAndOffset[i].offset = textureStartOffset;

          //  t->textureSizeAndStartBit[i] = tmp;//(m_Textures[i].size << 64) | (textureStartOffset << 32) | (type << 16) | (mime);
            textureStartOffset += m_Textures[i].size;
            //textureStartOffset += TEX_MARGIN;
        }
        return (char*)t;
    }
}