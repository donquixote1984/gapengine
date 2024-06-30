#pragma once

#include <string>
#include "Meta.h"
#include "AssetFile.h"

using namespace std;

namespace assetpacker {
    class Reader
    {
    protected:
        std::string m_Dir;
        AssetFile * m_AssetFile;
        Model m_Models[MAX_MODEL_COUNT];
        Texture m_Textures[MAX_TEXTURE_CHANNELS];
        int m_ModelCount = 0;
        int m_TextureCount = 0;
    public:
        Reader(const std::string path);
        virtual bool Validate();
        virtual void Read() = 0;
        void operator >> (AssetFile *as);
        char *ToHeaderBinary( int & size);
    };
}