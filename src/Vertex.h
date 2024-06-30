#pragma once
#include <glm/glm.hpp>
#include <vector>
#include <memory>
#include <initializer_list>

struct Vertex
{
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 uv;
};

typedef Vertex Vertex332f;

struct Vertex3f {
    glm::vec3 position;
};

struct Vertex33f
{
   glm::vec3 attr1; 
   glm::vec3 attr2;
};
struct Vertex333f
{
    glm::vec3 attr1;
    glm::vec3 attr2;
    glm::vec3 attr3;
};

struct Vertex22f
{
    glm::vec2 attr1;
    glm::vec2 attr2;
};


struct PointCloud
{
    std::shared_ptr<Vertex []> vertices;
    int count;
};

