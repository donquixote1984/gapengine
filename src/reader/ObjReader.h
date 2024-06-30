#pragma once
#include <vector>
#include "glm/glm.hpp"
#include <glm/gtc/type_ptr.hpp>
#include <tuple>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <filesystem>
#include "../alg/Algorithm.h"
#include "../geometry/Geometry.h"
#include "../animation/Bone.h"
#include "../render/BasicMaterial.h"
#include "../global/GlobalData.h"
#include "../Mesh.h"
#include "../MeshAssetReader.h"
#include "BoneExtractor.h"
#include "../Vertex.h"
#include "../util/Util.h"
#include "../util/JsonUtil.h"
#include "ObjPreProcessing.h"
#include "../exceptions/ResourceNotFoundException.h"

enum class VType {POSITION, NORMAL, TRIANGLE, UV, RAW};

class ObjReader
{
private:
    int m_VertexCount=0;
    float m_UnitScale = 1;
    const static int MAX_INDICES = 100000;
    std::vector<Mesh> m_Meshes;
    std::vector<std::unordered_map<std::string, Bone *>> m_Bones;
    VType GetDataType(std::string type);
    bool ByPassLineStartsWith(std::string line);
    std::string m_ObjPath;
    std::string m_ObjName = "Default";
    bool m_HasPreProcessing = false;
    ObjPreProcessing m_OPP;
    void ProcessNode(aiNode *node, const aiScene *scene);
    void ProcessAnimation(const aiScene * scene);
    void ProcessBones(const aiScene *scene);
    void ProcessTextures();
    void ProcessVertexWeight();
    void CheckFile(const std::string &filepath);
public:
    ObjReader(const std::string &filepath, const float unitscale = 1.0f);
    ObjReader(const std::string &filepath, ObjPreProcessing opp);
    ~ObjReader();

    void LoadModel();
    std::vector<Mesh> GetMeshes();
   // float * GetVertices(int &vsize, std::byte &layouts);
    void operator >> (Geometry *g);
    //void FeedTo(Geometry *g, );

};