#include "SceneAnimation.h"


SceneAnimation::SceneAnimation(aiAnimation * animation, aiNode * rootNode)
{
    m_Animation = animation;
    //m_GlobalInvertMat = glm::inverse(AssimpGLMHelpers::ConvertMatrixToGLMFormat(scene->mRootNode->mTransformation));
    m_Duration = animation->mDuration;
    m_TicksPerSecond = animation->mTicksPerSecond;
    //m_RootNode = rootNode;
    ReadHeirarchyData(m_RootNode, rootNode);
    ReadBones();
}

SceneAnimation::~SceneAnimation()
{
    for (auto &kv: m_BoneAnimations)
    {
        delete kv.second;
        kv.second = nullptr;
    }
    m_BoneAnimations.clear();
}


BoneAnimation* SceneAnimation::FindBone(const std::string& name)
{
    return m_BoneAnimations[name];
}

void SceneAnimation::ReadBones()
{
    int size = m_Animation->mNumChannels;

    //auto& boneInfoMap = model.GetBoneInfoMap();//getting m_BoneInfoMap from Model class
    //int& boneCount = model.GetBoneCount(); //getting the m_BoneCounter from Model class

    //reading channels(bones engaged in an animation and their keyframes)
    for (int i = 0; i < size; i++)
    {
        auto channel = m_Animation->mChannels[i];
        std::string boneName = channel->mNodeName.data;

        
        /*if (boneInfoMap.find(boneName) == boneInfoMap.end())
        {
            boneInfoMap[boneName].id = boneCount;
            boneCount++;
        }*/
        BoneAnimation *ba = new BoneAnimation(channel->mNodeName.data, channel);
        m_BoneAnimations[boneName] = ba;
        //m_BoneAnimations.push_back(new BoneAnimation(channel->mNodeName.data, m_BoneInfoMap[channel->mNodeName.data].id, channel));
    }

   // m_BoneInfoMap = boneInfoMap;
}

void SceneAnimation::ReadHeirarchyData(AssimpNodeData& dest, const aiNode* src)
{
    assert(src);

    dest.name = src->mName.data;
    dest.transformation = AssimpGLMHelpers::ConvertMatrixToGLMFormat(src->mTransformation);
    dest.childrenCount = src->mNumChildren;

    for (int i = 0; i < src->mNumChildren; i++)
    {
        AssimpNodeData newData;
        ReadHeirarchyData(newData, src->mChildren[i]);
        dest.children.push_back(newData);
    }
}

std::unordered_map<std::string, BoneAnimation*> SceneAnimation::GetBoneAnimations()
{
    return m_BoneAnimations;
}