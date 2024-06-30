#pragma once
#include <string>
#include <boost/algorithm/string/predicate.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <filesystem>
#include <vector>
#include "../exceptions/ResourceNotFoundException.h"
#include "../exceptions/ErrorContentException.h"
#include "../util/Util.h"
#include "../util/AssimpGLMHelpers.h"
#include "../animation/Bone.h"
#include "../animation/SceneAnimation.h"
#include "ObjPreProcessing.h"

class Geometry;

class AnimationReader
{
private:
    void CheckFile(const std::string &path);
    std::string m_ObjPath;
    ObjPreProcessing m_OPP;
    bool m_HasPreProcessing = false;
    std::string m_ObjName = "Default";
    void LoadModel();
    void ProcessAnimations(const aiScene * scene);
    aiAnimation * m_Animation;
    float m_UnitScale = 1;
    AssimpNodeData m_RootNode;
    std::vector<Bone> m_Bones;
    std::vector<SceneAnimation *> m_SceneAnimations;
public:
    AnimationReader(const std::string &path, ObjPreProcessing opp);
    void operator >> (Geometry * g);
    std::vector<SceneAnimation *> GetSceneAnimations();
};