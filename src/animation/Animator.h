#pragma once
#include "SceneAnimation.h"

class Animator
{   
public:
    Animator();
    void UpdateAnimation(float dt);

    void PlayAnimation(SceneAnimation* pAnimation);
    void CalculateBoneTransform(const AssimpNodeData* node, glm::mat4 parentTransform);
    std::vector<glm::mat4> GetFinalBoneMatrices();
    void SetCurrentSceneAnimation(SceneAnimation * animation);

private:
    std::vector<glm::mat4> m_FinalBoneMatrices;
    SceneAnimation* m_CurrentSceneAnimation;
    float m_CurrentTime = 0.0f;
    float m_DeltaTime;
};

