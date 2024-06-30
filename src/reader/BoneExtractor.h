#pragma once
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <unordered_map>
#include <vector>
#include "../animation/Bone.h"
#include "../util/AssimpGLMHelpers.h"

// from aiMesh to std::vector<Bone>
class BoneExtractor
{
private:
    std::vector<std::unordered_map<std::string, Bone *>> m_Bones;  // per mesh
    std::unordered_map<std::string, Bone *>  ReadMeshBone(aiMesh *mesh);
public:
    BoneExtractor(const aiScene *scene);
    std::vector<std::unordered_map<std::string, Bone *>> GetBones();
};