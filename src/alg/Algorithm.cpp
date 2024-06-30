#include "Algorithm.h"

Mesh alg::Scatter(Mesh mesh, int n, int seed)
{
    // randomly get n vertices
    // 1 select n face by random face index
    // 2 random vertices on n face.
    srand(seed + n);
    float *arr = (float*)alloca(n * sizeof(float));
    float * facePercentageArr = mesh.GetMeta().facesAreaList.get();
    Face * faces = mesh.GetFaces();

    int *targetTri = (int*)alloca(n * sizeof(int));
    //auto scatters = std::make_shared<glm::vec3[]>(10);
    //auto shared = std::make_shared<int[]>(10);
    std::shared_ptr<Vertex333f []> scatters(new Vertex333f[n]);
    // use 333 layout
    // for both Mesh{pos, normal, tex} and Transform{pos, rot, scale}
    Vertex333f *scatterPoints = scatters.get();
    for(int k = 0; k < n; k++)
    {
        arr[k] = (float)((float)rand() / RAND_MAX);
        int targetFaceIndex= AlgUtil::FindNumberInAreaArr(facePercentageArr, arr[k], 0, mesh.GetFacesCount()-1);
        if (targetFaceIndex == -1)
        {
            scatterPoints[k].attr1 = mesh.GetPosition(0);
            //scatterPoints[k].position = mesh.vertices[0].normal;
            continue;
        }
        Face targetFace = faces[targetFaceIndex];
        int *faceIndices = targetFace.indices.get();

        if (targetFace.indicesCount < 3)
        {
            Vertex v = mesh.GetPositionAndNormal(faceIndices[0]);
            scatterPoints[k].attr1= v.position;
            scatterPoints[k].attr2= v.normal;
            continue;
        }

        Vertex v0 = mesh.GetPositionAndNormal(faceIndices[0]);
        Vertex v1 = mesh.GetPositionAndNormal(faceIndices[1]);
        Vertex v2 = mesh.GetPositionAndNormal(faceIndices[2]);

        float u = ((float)rand() / RAND_MAX);
        float v = ((float)rand() / RAND_MAX);

        if (u + v > 1)
        {
            u = 1 - u;
            v = 1 - v;
        }

        float w = 1 - u - v;

        scatterPoints[k].attr1 = v0.position * u + v1.position * v + v2.position * w;
        glm::vec3 nor = glm::normalize(v0.normal+ v1.normal+ v2.normal);
        scatterPoints[k].attr2 = nor;
        scatterPoints[k].attr3 = glm::vec3(0.0f);
    };
    
    //std::static_pointer_cast<float>(scatters);
    //return scatters;
    std::shared_ptr<float[]> arr1 = std::reinterpret_pointer_cast<float[]>(scatters);
    Mesh result(arr1, n, {3,3,3});
  //  return result ;
    //Mesh result(std::);
    return result;
  //return M
}