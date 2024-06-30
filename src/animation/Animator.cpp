#include "Animator.h"

Animator::Animator()
{
    m_CurrentTime = 0.0;
    m_FinalBoneMatrices.reserve(100);

    for (int i = 0; i < 100; i++)
        m_FinalBoneMatrices.push_back(glm::mat4(1.0f));
}
void Animator::UpdateAnimation(float dt)
{
    m_DeltaTime = dt;
    if (m_CurrentSceneAnimation)
    {
        m_CurrentTime += m_CurrentSceneAnimation->GetTicksPerSecond() * dt;
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
            assert(0);
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

std::vector<glm::mat4> Animator::GetFinalBoneMatrices() 
{ 
    return m_FinalBoneMatrices;  
}

void Animator::SetCurrentSceneAnimation(SceneAnimation * anmiation)
{
    //m_CurrentTime = 0.0;
    m_CurrentSceneAnimation = anmiation;
}