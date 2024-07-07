#include "MultiMatReader.h"
#include "ReaderException.h"
namespace assetpacker {
    MultiMatReader::MultiMatReader(std::string path) : Reader(path)
    {}


    bool MultiMatReader::ValidateModel()
    {
        for (const auto& entry : filesystem::directory_iterator(m_Dir))
        {
            std::string path = entry.path().string();
            if (isModel(path))
            {
                m_ModelPath = path;
                m_ModelCount = 1;
                Model m = GetModelStruct(m_ModelPath);
                m_Models[0] = m;
                return true;
            }
        }
        throw ReaderException("no fbx files in folder");
    }

    void MultiMatReader::ProcessNode(const aiScene* scene)
    {
        for (int i = 0; i < scene->mNumMeshes; i++)
        {
            aiMesh* mesh = scene->mMeshes[i];
            aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

            //material->GetTextureCount(aiTextureType_DIFFUSE)
            ValidateTextureChannel(material, aiTextureType_DIFFUSE);
            ValidateTextureChannel(material, aiTextureType_SPECULAR);
            ValidateTextureChannel(material, aiTextureType_NORMALS);
            ValidateTextureChannel(material, aiTextureType_DIFFUSE_ROUGHNESS);
            ValidateTextureChannel(material, aiTextureType_SHININESS);
            ValidateTextureChannel(material, aiTextureType_METALNESS);
        }
    }
    bool MultiMatReader::ValidateTextures()
    {
        if (m_ModelPath.empty())
        {
            throw ReaderException("empty model path");
        }

        ReadFolderTextures();
        Assimp::Importer importer;
        const aiScene* scene = importer.ReadFile(m_ModelPath, aiProcess_GlobalScale);
        ProcessNode(scene);

    }

    void MultiMatReader::AddTextures(Texture t)
    {
        m_Textures[m_TextureCount] = t;
        m_TextureCount += 1;
    }

    void MultiMatReader::ValidateTextureChannel(aiMaterial* mat, aiTextureType t)
    {
        int channelNum = mat->GetTextureCount(t);
        aiString aiMatName = mat->GetName();
        std::string matName = aiMatName.C_Str();
        if (channelNum > 0)
        {
            aiString aiPath;
            mat->GetTexture(t, 0, &aiPath);
            std::string path = aiPath.C_Str();
            std::filesystem::path p = path;
            std::string textureName = p.filename().string();

            if (m_FolderTextures.find(textureName) == m_FolderTextures.end())
            {
                std::string msg = fmt::format("texture {} does not exist in material {}", textureName, matName);
                throw ReaderException(msg.c_str());
            }

            AddTextures(m_FolderTextures[textureName]);
        }

    }



    bool MultiMatReader::Validate()
    {
        if (!Reader::Validate())
        {
            return false;
        }

        ValidateModel();
        ValidateTextures();
    }

    void MultiMatReader::Read()
    {
        if (!Validate())
        {
            return;
        }

    }

    void MultiMatReader::ReadFolderTextures()
    {
        for (const auto& entry : filesystem::directory_iterator(m_Dir))
        {
            std::string path = entry.path().string();
            std::string name = entry.path().filename().string();
            if (isTexture(path))
            {
                Texture t = GetTexStruct(path);
                m_FolderTextures[name] = t;
            }
        }
    }

    char* MultiMatReader::ToHeaderBinary(int& size)
    {
        //   | - mcount |  m1 size + startbit|  m2 size + startbit|...| m10 size | color texture size + type[ type at low 4bit] | normal texture size | rough texture size | metal texturesize | 
        //
        //

        HeaderStruct* t = new HeaderStruct();

        t->mcount = min(m_ModelCount, MAX_MODEL_COUNT); // 1st   32
        t->tcount = min(m_TextureCount, MAX_TEXTURE_CHANNELS);

        SizeOffsetTypeMime modelSizeAndOffset[MAX_MODEL_COUNT];
        SizeOffsetTypeMime textureSizeAndOffset[MAX_TEXTURE_CHANNELS];


        int modelStartOffset = sizeof(HeaderStruct);

        for (int i = 0; i < m_ModelCount; i++)
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
            auto uri = m_Textures[i].uri;
            filesystem::path p = uri;
            string name = p.filename().string();

            t->textureSizeAndOffset[i].type = type;
            t->textureSizeAndOffset[i].mime = mime;
            t->textureSizeAndOffset[i].size = m_Textures[i].size;
            t->textureSizeAndOffset[i].offset = textureStartOffset;
            //t->textureSizeAndOffset[i].name = name.c_str();
            memcpy(t->textureSizeAndOffset[i].name, name.c_str(), name.size());

            //  t->textureSizeAndStartBit[i] = tmp;//(m_Textures[i].size << 64) | (textureStartOffset << 32) | (type << 16) | (mime);
            textureStartOffset += m_Textures[i].size;
            //textureStartOffset += TEX_MARGIN;
        }
        return (char*)t;
    }
}