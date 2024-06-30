#pragma once
#include <glm/glm.hpp>
#include <vector>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "Bone.h"
#include <unordered_map>
#include "../util/AssimpGLMHelpers.h"
#include "BoneAnimation.h"

struct AssimpNodeData
{
    glm::mat4 transformation;
    std::string name;
    int childrenCount;
    std::vector<AssimpNodeData> children;
};

class SceneAnimation
{
public:
    SceneAnimation() = default;
    SceneAnimation(aiAnimation * ani, aiNode *rootNode);
    ~SceneAnimation();

    BoneAnimation* FindBone(const std::string& name);
    inline int GetTicksPerSecond() { return m_TicksPerSecond; }
    inline float GetDuration() { return m_Duration;}
    inline const AssimpNodeData& GetRootNode() { return m_RootNode; }
    std::unordered_map<std::string, BoneAnimation*> GetBoneAnimations();
     glm::mat4 m_GlobalInvertMat;
private:
    void ReadBones();
    void ReadHeirarchyData(AssimpNodeData& dest, const aiNode* src);
    float m_Duration;
    int m_TicksPerSecond;
    aiAnimation *m_Animation;
    std::unordered_map<std::string, BoneAnimation*> m_BoneAnimations;
    AssimpNodeData m_RootNode;
};