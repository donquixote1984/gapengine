#pragma once
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "stb/stb_image.h"
#include "Mesh.h"
#include <array>
#include <unordered_map>
#include "ui/UISettings.h"
#include "textures/TextureType.h"
#include "textures/MemoryTextureData.h"
#include "util/AssimpGLMHelpers.h"
#include "reader/BoneExtractor.h"
#include "animation/Bone.h"

class MeshAssetReader
{
private:
    static void ProcessVertex(Mesh &m, aiMesh *aiMesh)
    {

        settings::UISettings::GetSettings()->verticesCount += aiMesh->mNumVertices;
         for (unsigned int i = 0; i < aiMesh->mNumVertices; i++) {
            std::array<float, 22> v = {
                aiMesh->mVertices[i].x, aiMesh->mVertices[i].y, aiMesh->mVertices[i].z,  // position
                aiMesh->mNormals[i].x, aiMesh->mNormals[i].y, aiMesh->mNormals[i].z, // normal
                aiMesh->mTextureCoords[0][i].x, aiMesh->mTextureCoords[0][i].y, // uvtexture
                aiMesh->mTangents[i].x, aiMesh->mTangents[i].y, aiMesh->mTangents[i].z, //tanglent
                aiMesh->mBitangents[i].x, aiMesh->mBitangents[i].y, aiMesh->mBitangents[i].z, // bitangent
                -1.0f, -1.0f, -1.0f, -1.0f, // bone ids
                .0f, .0f, .0f, .0f // bone weights
            };
            m.AddVertex(v);
        }
    }
    static void ProcessFaces(Mesh &m , aiMesh *aiMesh)
    {
        //Face * faces = m.GetFaces();
        for (unsigned int i = 0; i < aiMesh->mNumFaces; i++)
        {
            aiFace face = aiMesh->mFaces[i];
            m.AppendFaceIndices(i, face.mIndices, face.mNumIndices);
        }
    }

    static void ProcessFacePercentage(Mesh &m)
    {
        m.CalculateFaceAreaPercentage();
    }
    static void ProcessIndices(Mesh &m)
    {
        m.CalculateIndices();
    }

    static void ProcessEmbedTextures(Mesh &m, aiMesh *aiMesh, const aiScene * aiScene)
    {
        if (aiMesh->mMaterialIndex >= 0 && aiScene->mNumMaterials > 0)
        {
            aiMaterial *mat = aiScene->mMaterials[aiMesh->mMaterialIndex];
            MemoryTextureData diffuse = ProcessEmbedTexturesByType(mat, aiTextureType_DIFFUSE, aiScene);

            if (diffuse.data != nullptr) {
                m.AddEmbedTextures(TextureType::DIFFUSE_TEXTURE, diffuse);
            }
            MemoryTextureData specular = ProcessEmbedTexturesByType(mat, aiTextureType_SPECULAR, aiScene);
            if (specular.data != nullptr)
            {
                m.AddEmbedTextures(TextureType::SPECULAR_TEXTURE, specular);
            }
            MemoryTextureData normal = ProcessEmbedTexturesByType(mat, aiTextureType_NORMALS, aiScene);
            if (normal.data != nullptr)
            {
                m.AddEmbedTextures(TextureType::NORMAL_TEXTURE, normal);
            }
            MemoryTextureData roughness = ProcessEmbedTexturesByType(mat, aiTextureType_DIFFUSE_ROUGHNESS, aiScene);
            if (roughness.data != nullptr)
            {
                m.AddEmbedTextures(TextureType::ROUGHNESS_TEXTURE, roughness);
            } else {
                // use shininess for backup
                roughness = ProcessEmbedTexturesByType(mat, aiTextureType_SHININESS, aiScene);
                if (roughness.data != nullptr)
                {
                    m.AddEmbedTextures(TextureType::ROUGHNESS_TEXTURE, roughness);
                }
            }

            MemoryTextureData metalness = ProcessEmbedTexturesByType(mat, aiTextureType_METALNESS, aiScene);
            if (metalness.data != nullptr)
            {
                m.AddEmbedTextures(TextureType::METALNESS_TEXTURE, metalness);
            }
            ProcessEmbedTexturesByType(mat, aiTextureType_UNKNOWN, aiScene);
        }

    }
    static MemoryTextureData ProcessEmbedTexturesByType(aiMaterial *mat, aiTextureType type, const aiScene *scene)
    {
        // just load 1 texture per type for short;
        if (mat->GetTextureCount(type) <= 0)
        {
            return {};
        }
        aiString imagePath;
        mat->GetTexture(type, 0, &imagePath);
        auto tex = scene->GetEmbeddedTexture(imagePath.C_Str());
        if (tex == nullptr)
        {
            // no embed texture. just return;
            return {};
        }
        // no need to free(tex),   the data is still management by assimp and will be correctly deconstructed.

        int width, height, channels;
        unsigned char * data = nullptr;
        if (tex->mHeight == 0) 
        {
            data = stbi_load_from_memory(reinterpret_cast<unsigned char*>(tex->pcData), tex->mWidth, &width, &height, &channels, 4);
        }
        else 
        {
		    data = stbi_load_from_memory(reinterpret_cast<unsigned char*>(tex->pcData), tex->mWidth * tex->mHeight, &width, &height, &channels, 4);
        }
        // this data need to be freed after pumping to Texture
        return {data, width, height, channels};
    }
   
public:
    static Mesh ReadMesh(aiMesh *aiMesh, std::string name, const aiScene * aiScene)
    {

        Logger *l = Logger::GetLogger();

        Mesh m(aiMesh->mNumVertices, {3,3,2,3,3,4,4});
        m.SetName(name);
        m.InitFace(aiMesh->mNumFaces);
        ProcessVertex(m, aiMesh);
        ProcessFaces(m, aiMesh);
        ProcessFacePercentage(m);
        ProcessIndices(m);
        ProcessEmbedTextures(m, aiMesh, aiScene);
        return m;
    }

};