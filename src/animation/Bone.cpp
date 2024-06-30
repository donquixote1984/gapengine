#include "Bone.h"

Bone::Bone(std::string name,  unsigned int id, glm::mat4 offset): m_Name(name), m_ID(id), m_Offset(offset)
{

}

 void Bone::AddWeights(unsigned int vertexId, float weight)
 {
   if (m_Weights.find(vertexId) == m_Weights.end())
   {
      m_Weights[vertexId] = weight;
   } else 
   {
      assert(0);
   }
 }

 std::unordered_map<unsigned int, float> Bone::GetWeights()
 {
    return m_Weights;
 }

 unsigned int Bone::GetId()
 {
    return m_ID;
 }

 glm::mat4 Bone::GetOffset()
 {
   return m_Offset;
 }