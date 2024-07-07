#include "Util.h"
int util::RandInRange(int from , int to)
{
    std::random_device rd; // obtain a random number from hardware
    std::mt19937 gen(rd()); // seed the generator
    std::uniform_int_distribution<> distr(from, to); // define the range
    return distr(gen);
}

float util::RandInRange(float from , float to)
{
    std::random_device rd; // obtain a random number from hardware
    std::mt19937 gen(rd()); // seed the generator
    std::uniform_real_distribution<> distr(from, to); // define the range
    return (float)distr(gen);
}

float util::TriArea(glm::vec3 v1, glm::vec3 v2, glm::vec3 v3)
{
    float A= length(v2 - v1);
    float B= length(v3 - v1);
    float C= length(v3 - v2);

    float S = (A + B + C) / 2;
    return sqrt(S * (S-A) * (S - B) * (S - C));
    //float angle_AB_AC = glm::dot(AB, AC);
    //return .5 * glm::length(AB) * glm::length(AC) * sin(angle_AB_AC);
}

std::bitset<32> util::ILToByte(std::initializer_list<int> layouts)
{
    std::bitset<32> bLayout = std::bitset<32> {"0000000000000000"};
    int iter = 0;

    for (const auto i: layouts)
    {
        std::bitset<32> tmp = (i & 7)<< iter;
        bLayout |= tmp;
        iter += 3;
    }

    return bLayout;
}

std::ostream& operator<<(std::ostream& os, std::byte b)
{
    return os << std::bitset<8>(std::to_integer<int>(b));
}

int util::FileCount(const std::string dir)
{
    auto dirIter = std::filesystem::directory_iterator(dir);
    return std::count_if(begin(dirIter), end(dirIter),
    [](auto& entry) { return entry.is_regular_file(); }
);
}

std::string util::ReadFile(const std::string &filepath)
{
    std::ifstream ifs(filepath);
    std::ostringstream oss;
    oss << ifs.rdbuf();
    ifs.close();
    return oss.str();
}
std::string util::GetContainerFolder(const std::string &filepath)
{
    return std::filesystem::path(filepath).parent_path().string();
}

int util::GetLayoutSize(std::bitset<32> layout)
{
    std::bitset<32> tmp = layout;
    int size = 0;
    for (int i = 0; i < 10; i+=1)
    {
        if ((tmp | std::bitset<32>{"0"}) == 0)
        {
            return size;
        } else {
            size += 1;
        }
        tmp = (tmp >> 3);
    }
    return size;
}
std::string util::GetFileNameByPath(const std::string& path)
{
    std::filesystem::path p = path;
    return p.filename().string();
}