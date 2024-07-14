#include "Animator.h"
#include "../Config.h"

Animator::Animator()
{
    m_CurrentTime = 0.0;
    m_FinalBoneMatrices.reserve(constants::MAX_BONE_NUM);

    for (int i = 0; i < constants::MAX_BONE_NUM; i++)
        m_FinalBoneMatrices.push_back(glm::mat4(1.0f));
}
void Animator::UpdateAnimation(float dt, bool additive)
{
    m_DeltaTime = dt;
    if (m_CurrentSceneAnimation)
    {
        if (additive)
        {
            m_CurrentTime += m_CurrentSceneAnimation->GetTicksPerSecond() * dt * m_Timefactor;
        }
        m_CurrentTime = fmod(m_CurrentTime, m_CurrentSceneAnimation->GetDuration());
        CalculateBoneTransform(&m_CurrentSceneAnimation->GetRootNode(), glm::mat4(1.0f));
    }
}

void Animator::PlayAnimation(SceneAnimation* pAnimation)
{
    m_CurrentSceneAnimation = pAnimation;
    m_CurrentTime = 0.0f;
}
void Animator::CalculateBoneTransform(const AssimpNodeData* node, glm::mat4 parentTransform)
{
    std::string nodeName = node->name;
    glm::mat4 nodeTransform = node->transformation;

    BoneAnimation* boneAnimation = m_CurrentSceneAnimation->FindBone(nodeName);
    Bone * bone = nullptr;
    
    if (boneAnimation)
    {
        boneAnimation->Update(m_CurrentTime);
        nodeTransform = boneAnimation->GetLocalTransform();
        bone = boneAnimation->GetBone();
        if (bone == nullptr)
        {
            //assert(0);
        }
    }

    glm::mat4 globalTransformation = parentTransform * nodeTransform;

    //std::unordered_map<std::string, BoneTransform> boneInfoMap = m_CurrentSceneAnimation->GetBoneIDMap();

    if (bone)
    {
        int index = bone->GetId();
        glm::mat4 offset = bone->GetOffset();
        m_FinalBoneMatrices[index] = globalTransformation * offset ;
    } 

    for (int i = 0; i < node->childrenCount; i++)
        CalculateBoneTransform(&node->children[i], globalTransformation);
}

std::vector<glm::mat4> & Animator::GetFinalBoneMatrices() 
{ 
    return m_FinalBoneMatrices;  
}

glm::mat4* Animator::GetFinalBoneMatricesPtr(unsigned int& count)
{
    count = m_FinalBoneMatrices.size();
    return m_FinalBoneMatrices.data();
}

void Animator::SetCurrentSceneAnimation(SceneAnimation * anmiation)
{
    //m_CurrentTime = 0.0;
    m_CurrentSceneAnimation = anmiation;
}

void Animator::SetTimeFactor(float factor)
{
    m_Timefactor = factor;
}
