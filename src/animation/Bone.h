#pragma once
#include <string>
#include <glm/glm.hpp>
#include <unordered_map>
class Bone
{
private:
    std::string m_Name;
    glm::mat4 m_Offset = glm::mat4(1.0f);
    unsigned int m_ID;
    std::unordered_map<unsigned int, float> m_Weights;

public:
    Bone(std::string name, unsigned int id, glm::mat4 offset);
    void AddWeights(unsigned int vertexId, float weight);
    std::unordered_map<unsigned int, float> GetWeights();
    unsigned int GetId();
    glm::mat4 GetOffset();
};