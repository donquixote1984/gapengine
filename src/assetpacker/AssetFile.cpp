#include "AssetFile.h"
namespace assetpacker {
    void AssetFile::CreateFile()
    {
        std::string timebuffer = GetTimeMills();
        timebuffer += ".asset";
        m_Filename = timebuffer;
        ofstream outfile(m_Filename, std::ios::out | std::ios::binary);
    // const char * header = "133782B";
    //  int header = HEADER_SIGN;
    //  outfile.write((const char*)&header, sizeof(header));
        outfile.flush();
        outfile.close();
    }

    void AssetFile::AppendHeader(char * startptr)
    {
         ofstream outfile;
        outfile.open(m_Filename, std::ios::out | std::ios::binary | std::ios::app);
        outfile.write(startptr, sizeof(HeaderStruct));
        outfile.close();
    }

    void AssetFile::AppendModel(Model *model, int count)
    {
        ofstream outfile;
        outfile.open(m_Filename, std::ios::out | std::ios::binary | std::ios::app);
        for (int i = 0; i < count ; i++)
        {
            Model m = model[i];
            ifstream input;
            input.open(m.uri, std::ios::in | std::ios::binary);
            outfile << input.rdbuf();
            input.close();
        }

        outfile.close();
    }

    void AssetFile::AppendTexture(Texture * texture, int count)
    {
        ofstream outfile;
        outfile.open(m_Filename, std::ios::out | std::ios::binary | std::ios::app);
        for(int i = 0; i < count; i++)
        {
            Texture t = texture[i];
            ifstream input;
            input.open(t.uri, std::ios::in | std::ios::binary);
            outfile << input.rdbuf();
                       input.close();
        }
        outfile.close();
    }

    void AssetFile::Append(ifstream &input, int contentSize)
    {
        ofstream outfile;
        outfile.open(m_Filename, std::ios::out | std::ios::binary);
        outfile << input.rdbuf();
        outfile.close();
        input.close();
    }

    bool AssetFile::Created()
    {
        if (m_Filename.empty())
        {
            return false;
        }
        return true;
    }

    void AssetFile::AddModelMeta()
    {

    }

    void AssetFile::AddTextureMeta()
    {

    }

    void AssetFile::ReadHeader(ifstream &input)
    {
        HeaderStruct t;
        input.seekg(sizeof(int)); // bypass header;
        input.read((char *)&t, sizeof(HeaderStruct));
        std::cout << ((int)t.mcount) << std::endl;
        //return header;
    }

    std::string AssetFile::GetFilePath()
    {
        return m_Filename;
    }

    const void AssetFile::operator >> (AssetFileExtractor *afe)
    {
        afe->Extract(m_Filename);
    }
}