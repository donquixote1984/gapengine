#pragma once
#include <memory>
#include <math.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "../../Transform.h"
#include "../../util/Util.h"

#include "PointGeneratorDistributionType.h"

enum class PointsGeneratorMode
{
    POSITION = 0x001,
    ROTATION = 0x010,
    SCALE = 0x100,
};


class PointsGenerator
{
private:
    Transform m_Pivot;
    int m_Count;
    float m_ScaleVariant = 0;
    glm::vec3 m_RotationVariant = {0, 1, 0};
    void AssignRS(Transform &t);
    void PointInCircle(Transform &t, float radius);
    void PointInSquare(Transform &t, float radius);
public:
    PointsGenerator(int count);
    PointsGenerator(int count, Transform pivot);
    std::shared_ptr<Transform[]>  Circle(float radius);
    std::shared_ptr<glm::mat4[]>  CircleMat(float radius);
    std::shared_ptr<Transform[]>  Square(float edgeLength);
    std::shared_ptr<glm::mat4[]>  SquareMat(float edgeLength);
    std::shared_ptr<glm::mat4[]>  GridMat(int rows, int columns, float interval, glm::vec3 up, glm::vec3 forward, glm::vec3 offset);
    void SetScaleVariant(float scale);
};