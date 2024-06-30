#include "AnimationReader.h"
#include "../geometry/Geometry.h"
#include "../global/GlobalData.h"
#include "../util/Util.h"


AnimationReader::AnimationReader(const std::string &path,  ObjPreProcessing opp)
{
    CheckFile(path);
    m_OPP = opp;
    m_HasPreProcessing = true;
    if (Global::animationMap.find(path) == Global::animationMap.end())
    {
        LoadModel();
    }
}
void AnimationReader::LoadModel()
{
    Assimp::Importer import;
    if (m_HasPreProcessing) {
        m_UnitScale = m_OPP.unitScale;
    }
    import.SetPropertyFloat(AI_CONFIG_GLOBAL_SCALE_FACTOR_KEY, m_UnitScale);
    // if it is a fbx ,  enable this settings
    if (boost::algorithm::ends_with(m_ObjPath, ".fbx"))
    {
        import.SetPropertyBool(AI_CONFIG_IMPORT_FBX_PRESERVE_PIVOTS, false);
    }
    const aiScene *scene = import.ReadFile(m_ObjPath, aiProcess_Triangulate | aiProcess_GlobalScale); 
    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
        throw ErrorContentException(m_ObjPath.c_str());
        return;
    }

    ProcessAnimations(scene);
}

void AnimationReader::ProcessAnimations(const aiScene * scene)
{
    aiNode *root = scene->mRootNode;
    for (unsigned int i = 0; i < scene->mNumAnimations; i++)
    {
        aiAnimation *ani = scene->mAnimations[i];
        SceneAnimation * animation = new SceneAnimation(ani, root);
        m_SceneAnimations.push_back(animation);
    }
}

void AnimationReader::CheckFile(const std::string &path)
{
    if (std::filesystem::exists(path))
    {
        m_ObjPath = path;
        m_ObjName = util::GetFileName(path);
        return;
    }

    if (std::filesystem::exists(constants::ASSET_INSTALL_PATH + path))
    {
        m_ObjPath = constants::ASSET_INSTALL_PATH + path;
        m_ObjName = util::GetFileName(path);
        return;
    }
     std::string msg = path + " not found";
    throw ResourceNotFoundException(msg.c_str());
}

void AnimationReader::operator >> (Geometry * g)
{
    g->FeedAnimations(m_SceneAnimations);
}

std::vector<SceneAnimation *> AnimationReader::GetSceneAnimations()
{
    return m_SceneAnimations;
}