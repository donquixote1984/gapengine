#pragma once

#include <vector>
#include <initializer_list>
#include <algorithm>
#include <memory>
#include <unordered_map>
#include "../alg/Algorithm.h"
#include "../util/Util.h"
#include "../Mesh.h"
#include "RawVertices.h"
#include "postprocess/DataPostProcessing.h"
#include "../animation/Bone.h"
#include "../animation/BoneAnimation.h"
#include "../animation/SceneAnimation.h"


enum class GeometryDataType
{
    MESHES,
    POINTCLOUD,
    EMPTY
};

class GeometryData
{
    friend class GeometryDataFactory;
protected:
    std::vector<Mesh> m_Meshes;
    std::vector<std::unordered_map<std::string, Bone *>> m_Bones;
    //std::vector<BoneAnimation*> m_Animations;
    std::vector<SceneAnimation*> m_SceneAnimations;
    
    bool m_IsEmpty = true;
    GeometryData();
    GeometryData(std::vector<Mesh> meshes);
    GeometryData(std::vector<Mesh> meshes, std::vector<std::unordered_map<std::string, Bone *>> bones);
    GeometryData(Mesh mesh);
    
    GeometryData(float *rawArr, int size, std::initializer_list<int> layouts);
    GeometryData(std::shared_ptr<float[]> rawArrSP, int size, std::initializer_list<int> layouts);
    Bone * FindBone(const std::string &name);
    
public:
    ~GeometryData();
    void FeedData(std::vector<Mesh> meshes);
    void FeedData(std::vector<std::unordered_map<std::string, Bone *>> bones);
    void FeedData(float *rawArr, int size, std::initializer_list<int> layouts);
    void FeedData(std::shared_ptr<float[]> rawArrSp, int vertexCount, std::initializer_list<int> layouts);
    void FeedData(Mesh mesh);
    bool IsEmpty();
    std::vector<Mesh> GetMeshes();
    int GetLayoutSize();
    bool HasBone();
    bool HasAnimation();
    void FeedSceneAnimation(std::vector<SceneAnimation *> animations);
    SceneAnimation * GetSceneAnimation(unsigned int index);

};

class GeometryDataFactory {
public:

    static GeometryData * CreateGeometryData(std::vector<Mesh> meshes, std::vector<std::unordered_map<std::string, Bone *>> bones)
    {
        return new GeometryData(meshes, bones);
    }
    static GeometryData * CreateGeometryData(std::vector<Mesh> meshes)
    {
        return new GeometryData(meshes);
    }
    static GeometryData * CreateGeometryData(Mesh mesh)
    {
        return new GeometryData(mesh);
    }
    static GeometryData * CreateGeometryData(float *arr, int count, std::initializer_list<int> layouts)
    {
        return new GeometryData(arr, count, layouts);
    }
    static GeometryData * CreateGeometryData(std::shared_ptr<float[]>arr, int count, std::initializer_list<int> layouts)
    {
        return new GeometryData(arr, count, layouts);
    }

    static GeometryData * CreateEmptyGeometryVariantData(int size )
    {
        return new GeometryData[size];
    }
};