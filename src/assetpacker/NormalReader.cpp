#include "NormalReader.h"

namespace assetpacker {
    NormalReader::NormalReader(std::string path) : Reader(path)
    {

    }

    void NormalReader::operator>> (AssetFile* assetFile)
    {
        if (!assetFile->Created())
        {
            assetFile->CreateFile();
        }

        m_AssetFile = assetFile;
        Read();

        int size = 0;
        char* startptr = ToHeaderBinary(size);
        //append header
        m_AssetFile->AppendHeader(startptr);
        m_AssetFile->AppendModel(m_Models, m_ModelCount);
        m_AssetFile->AppendTexture(m_Textures, m_TextureCount);
        delete startptr;
        // 
    };

    bool NormalReader::Validate()
    {
        if (!Reader::Validate())
        {
            return false;
        }

        for (const auto& entry : filesystem::directory_iterator(m_Dir))
        {
            std::string path = entry.path().string();
            if (isTexture(path))
            {
                // m_TexturePaths.push_back(path);
                Texture t = GetTexStruct(path);
                if (t.size == 0) {
                    continue;
                }
                else
                {
                    if (t.type == TextureType::UNSUPPORTED)
                    {
                        continue;
                    }
                    m_Textures[m_TextureCount] = t;
                    m_TextureCount += 1;
                    if (m_TextureCount > MAX_TEXTURE_CHANNELS)
                    {
                        return true;
                    }
                }
            }

            if (isModel(path))
            {

                //__int128_t size = filesystem::file_size(path);
                Model m = GetModelStruct(path);
                if (m.size == 0)
                {
                    continue;
                }
                else {
                    m_Models[m_ModelCount] = m;
                    m_ModelCount += 1;
                    if (m_ModelCount > MAX_MODEL_COUNT)
                    {
                        return true;
                    }
                }
                //m_ModelPaths.push_back(path);
            }
        }

        if (m_TextureCount > 0 || m_ModelCount > 0)
        {
            return true;
        }

        cout << "Directory: " << m_Dir << " does not contain any valid files. " << endl;
        return false;
    }

    void NormalReader::Read()
    {
        if (!Validate())
        {
            return;
        }

    }
}