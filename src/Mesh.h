#pragma once
#include <memory>
#include <initializer_list>
#include <unordered_map>
#include <glm/glm.hpp>
#include <bitset>
#include "Vertex.h"
#include "util/Util.h"
#include "Logger.h"
#include <array>
#include <sstream>
#include <fstream>
#include <unordered_map>
#include "textures/TextureType.h"
#include "textures/MemoryTextureData.h"
#include "Config.h"
#include "animation/Bone.h"

struct Face
{
    std::shared_ptr<int[]> indices;
    int indicesCount = 0;
    float area = 0;
    float areaPercentage = 0;
};

struct MeshMeta
{
    std::shared_ptr<float []> facesAreaList; // range list, calculate scatter chance, {0.01, 0.02, 0.342, 0.56, 0.8.1}
};

struct MeshEmbedTexture
{
    TextureType type;
    MemoryTextureData data;
};

class Mesh
{
private:
    std::shared_ptr<float []> m_Vertices;
    std::bitset<32> m_Layouts{"0000000000000000"};
    std::string m_Name = "Default";
    int m_VerticesCount = 0;
    int m_Stride = 0;
    //int *indices = NULL;
    int m_IndicesCount = 0;
    std::shared_ptr<int[]> m_Indices;
    std::shared_ptr<Face[]> m_Faces = NULL;
    int m_FacesCount = 0;
    int m_VerticesAdded = 0;
    float m_TotalArea = 0;
    MeshMeta m_Meta;
    bool m_HasFace = false;
    bool m_HasIndices = false;
    bool m_HasNormal = false;
    bool m_HasBone = false;

    std::vector<MeshEmbedTexture> m_EmbedTextures;
    //std::unordered_map<std::string, BoneTransform> m_Bones;

    unsigned int m_BoneCounter = 0;
public:
    Mesh();
    Mesh(int verticesCount, std::initializer_list<int> layout); //used b asset reader
    Mesh(std::shared_ptr<float[]> data, int verticesCount, std::initializer_list<int> layout);
    Mesh(std::shared_ptr<float[]> data, int verticesCount, std::bitset<32>layout);
    Mesh(std::shared_ptr<float[]> data, std::shared_ptr<int[]> indices, int verticesCount, int indiceCount, std::initializer_list<int> layouts);
    void AddVertex(std::array<float, 22> array);
    void AddVertexWeight(std::unordered_map<std::string, Bone *> boneMap);
    MeshMeta GetMeta();
    Vertex * GetVertex8f();
    Vertex GetPositionAndNormal(int indices);
    void InitFace(int);
    Face * GetFaces();
    void AppendFaceIndices(unsigned int faceIndex, unsigned int * indices, unsigned int indicesCount);
    glm::vec3 GetPosition(int indice);
    float * GetPositionPtr(int indice);
    void CalculateFaceAreaPercentage();
    void CalculateIndices();
    int GetFacesCount();
    bool HasNormal();
    void WithNormal(bool withNormal);
    float * GetVertices();
    int * GetIndices();
    int GetVerticesCount();
    std::bitset<32> GetLayout();
    int GetLayoutSize();
    bool HasIndices();
    int GetFaceCount();
    void DumpToFile();
    void Log();
    std::string ToString();
    void SetName(const std::string &name);
    int GetStride();
    void RebuildUV();
    void AddEmbedTextures(TextureType type, MemoryTextureData data);
    std::vector<MeshEmbedTexture> GetEmbedTextures();
   // std::unordered_map<std::string, BoneTransform> &GetBones();
    //unsigned int InsertBone(std::string name, glm::mat4 offset);
    //unsigned int GetBoneId(std::string name);
    void UpdateBoneData(int indices, unsigned int boneId, float weight);
    bool HasBone();
};
