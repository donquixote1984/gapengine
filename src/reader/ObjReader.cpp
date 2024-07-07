#include "ObjReader.h"
#include "../Utils.h"
#include "../util/JsonUtil.h"




ObjReader::ObjReader(const std::string &filepath, const float unitscale):m_UnitScale(unitscale)
{
    CheckFile(filepath);
    if (Global::geometryDataMap.find(filepath) == Global::geometryDataMap.end())
    {
        LoadModel();
    }
}


ObjReader::ObjReader(const std::string &filepath, ObjPreProcessing opp)
{
    CheckFile(filepath);
    m_HasPreProcessing = true;
    m_OPP = opp;

    if (Global::geometryDataMap.find(filepath) == Global::geometryDataMap.end())
    {
        LoadModel();
        return;
    }

}

void ObjReader::CheckFile(const std::string &filepath)
{
    if (std::filesystem::exists(filepath))
    {
        m_ObjPath = filepath;
        m_ObjName = util::GetFileName(filepath);
        return;
    }

    if (std::filesystem::exists(constants::ASSET_INSTALL_PATH + filepath))
    {
        m_ObjPath = constants::ASSET_INSTALL_PATH + filepath;
        m_ObjName = util::GetFileName(filepath);
        return;
    }
     std::string msg = filepath + " not found";
    throw ResourceNotFoundException(msg.c_str());

}


ObjReader::~ObjReader()
{}

void ObjReader::LoadModel()
{
    Assimp::Importer import;
    if (m_HasPreProcessing) {
        m_UnitScale = m_OPP.unitScale;
    }
    import.SetPropertyFloat(AI_CONFIG_GLOBAL_SCALE_FACTOR_KEY, m_UnitScale);

    const aiScene *scene = import.ReadFile(m_ObjPath, aiProcess_FlipUVs | aiProcess_Triangulate | aiProcess_GlobalScale | aiProcess_CalcTangentSpace); 
    //double factor;
    //int result = scene->mMetaData->Get("UnitScaleFactor", factor);

    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
        return;
    }
    ProcessNode(scene->mRootNode, scene);
    //ProcessAnimation(scene);  move to AnimationReader.h
    if (m_OPP.useAssetMaterial)
    {
        //ProcessMaterials(scene);
    }
    ProcessBones(scene);
    ProcessVertexWeight();
}

void ObjReader::ProcessBones(const aiScene *scene)
{
    BoneExtractor extractor(scene);
    m_Bones = extractor.GetBones();
}

void ObjReader::ProcessVertexWeight()
{
    for (int i = 0; i < m_Bones.size(); i++)
    {
        auto boneMap = m_Bones[i];
        if (boneMap.size() > 0)
        {
            Mesh &m = m_Meshes[i];
            //m.GetPositionPtr
            m.AddVertexWeight(boneMap);
        }
    }
}
void ObjReader::ProcessAnimation(const aiScene * scene)
{
    if (scene->mNumAnimations > 0) 
    {
    //    aiAnimation * animation = scene->mAnimations[0];
    //    Animation ani(animation, scene);
    //    m_Animations.push_back(ani);
        
    }

}
void ObjReader::ProcessNode(aiNode * node, const aiScene *scene)
{
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]]; 
        int count = scene->mNumTextures;
        Mesh m = MeshAssetReader::ReadMesh(mesh, m_ObjName, scene);
       // m.DumpToFile();
        m_Meshes.push_back(m);
    }

    for (unsigned int i = 0; i < node->mNumChildren; i++)
    {
        ProcessNode(node->mChildren[i], scene);
    }
}

void ObjReader::ProcessTextures()
{

}

void ObjReader::operator >> (Geometry *g)
{
    GeometryData *data;
    if (Global::geometryDataMap.find(m_ObjPath) != Global::geometryDataMap.end())
    {
        data = Global::geometryDataMap[m_ObjPath];
    } else {
        data = GeometryDataFactory::CreateGeometryData(m_Meshes, m_Bones);
        Global::geometryDataMap[m_ObjPath] = data;
    }
    /**
     *  data geometry data to runtime database here !
     * 
    */

   /**
    *  storage end.
    * 
   */
    data->SetUseAssetMaterial(m_OPP.useAssetMaterial);
    g->InitGeometryData(data);
}

/*
float * ObjReader::GetVertices(int &vcount, std::byte &layouts)
{
    //vsize = m_VertexIndices.size();
    //arr = new float[vsize];
    layouts = std::byte{0b00101111};

    int verticesCount = m_VertexIndices.size();
    vcount = verticesCount;
    int vsize = verticesCount * 3;

    if (hasNormal)
    {
        vsize = verticesCount * 6;
    }

    if (hasTex)
    {
        vsize = verticesCount * 8;
    }

    float * arr = new float[vsize];

    int index = 0;
    for (int i = 0;  i < m_VertexIndices.size(); i++)
    {
        glm::vec3 pos = m_Vertices[m_VertexIndices[i]];
        //v.SetPosition(pos);
        memcpy(arr + index, glm::value_ptr(pos), 3*sizeof(float));
        index += 3;

        if (hasNormal)
        {
            glm::vec3 nor = m_Normals.at(m_NormalIndices.at(i));
            memcpy(arr + index, glm::value_ptr(nor), 3*sizeof(float));
            index += 3;
        }

        if (hasTex)
        {
            glm::vec2 uv = m_UVs.at(m_UVIndices.at(i));
            memcpy(arr + index, glm::value_ptr(uv), 2*sizeof(float));
            index += 2;
        }
    }
    return arr;
}*/

bool ObjReader::ByPassLineStartsWith(std::string line)
{
    if (line[0] == '#') return true;
    if (line[0] == 's') return true;
    if (line[0] == 'o') return true;
    return false;
}

std::vector<Mesh> ObjReader::GetMeshes()
{
    return m_Meshes;
}