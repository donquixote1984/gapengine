#include <iostream>
#include <random>
#include <string>
#include <bitset>
#include <glm/glm.hpp>
#include <initializer_list>
#include <filesystem>
#include <fstream>
#include <sstream>
namespace util {

    int RandInRange(int from , int to);
    float RandInRange(float from , float to);
    float TriArea(glm::vec3 v1, glm::vec3 v2, glm::vec3 v3);
    std::bitset<32> ILToByte(std::initializer_list<int> il);
    int FileCount(const std::string dir);
    std::string ReadFile(const std::string &path);
    std::string GetContainerFolder(const std::string &path);
    int GetLayoutSize(std::bitset<32> layout);
    std::string GetFileNameByPath(const std::string& path);
};
std::ostream& operator<<(std::ostream& os, std::byte b);
