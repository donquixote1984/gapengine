#include "BoneExtractor.h"
BoneExtractor::BoneExtractor(const aiScene *scene)
{
    aiAnimation *animation = scene->mAnimations[0];
    int meshNum = scene->mNumMeshes;
    for (int i = 0; i < meshNum; i++)
    {
        // get bone map for each mesh
        aiMesh *mesh = scene->mMeshes[i];
        std::unordered_map<std::string, Bone *> bones = ReadMeshBone(mesh);
        //
        if (bones.size() > 0)
        {
            m_Bones.push_back(bones);
        }
    }
}

std::unordered_map<std::string, Bone *> BoneExtractor::ReadMeshBone(aiMesh * mesh)
{
    std::unordered_map<std::string, Bone *> cachedBone;
    unsigned int boneId = -1;
    int boneNum = mesh->mNumBones;

    for (int i = 0; i < boneNum; i++)
    {
        std::string boneName = mesh->mBones[i]->mName.C_Str();
        aiBone *aBone = mesh->mBones[i];
        if (cachedBone.find(boneName) == cachedBone.end())
        {
            // new bone
            boneId += 1;
            glm::mat4 offset = AssimpGLMHelpers::ConvertMatrixToGLMFormat(aBone->mOffsetMatrix);
            Bone *b = new Bone(boneName, boneId, offset); // deconstruct at geometrydata
            cachedBone[boneName] = b;
            unsigned int weightsNum = aBone->mNumWeights;
            for (int j = 0; j < weightsNum; j++)
            {
                aiVertexWeight aWeight = aBone->mWeights[j];
                b->AddWeights(aWeight.mVertexId, aWeight.mWeight);
            }
        } 
    }
    return cachedBone;
}

std::vector<std::unordered_map<std::string, Bone *>> BoneExtractor::GetBones()
{
    return m_Bones;
}
