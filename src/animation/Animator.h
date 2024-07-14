#pragma once
#include "SceneAnimation.h"

class Animator
{   
public:
    Animator();
    void UpdateAnimation(float dt, bool additive = true);

    void PlayAnimation(SceneAnimation* pAnimation);
    void CalculateBoneTransform(const AssimpNodeData* node, glm::mat4 parentTransform);
    std::vector<glm::mat4> &GetFinalBoneMatrices();
    glm::mat4 * GetFinalBoneMatricesPtr(unsigned int& count);
    void SetCurrentSceneAnimation(SceneAnimation * animation);
    void SetTimeFactor(float factor);

private:
    std::vector<glm::mat4> m_FinalBoneMatrices;
    SceneAnimation* m_CurrentSceneAnimation;
    float m_CurrentTime = 0.0f;
    float m_DeltaTime;
    float m_Timefactor = 1.0f;
};

