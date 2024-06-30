#include "PointsGenerator.h"

PointsGenerator::PointsGenerator(int count):m_Count(count)
{}
PointsGenerator::PointsGenerator(int count, Transform pivot): m_Count(count), m_Pivot(pivot)
{}

void PointsGenerator::PointInCircle(Transform &t, float radius)
{
    float r = radius * sqrt(util::RandInRange(0.0f, 1.0));
    float a = util::RandInRange(0.0f, (float)(M_PI * 2));

    float x = r * cos(a);
    float z = r * sin(a);
    float y = 0.0f;
    t.position = glm::vec3(x, y, z);
    AssignRS(t);
}

std::shared_ptr<Transform[]>  PointsGenerator::Circle(float radius)
{

    std::shared_ptr<Transform[]> ret = std::shared_ptr<Transform[]> (new Transform[m_Count]);

    for (int i = 0; i < m_Count ; i++)
    {
        PointInCircle(ret[i], radius);
    }

    return ret;
}

std::shared_ptr<glm::mat4[]>  PointsGenerator::CircleMat(float radius)
{
    std::shared_ptr<glm::mat4 []> ret = std::shared_ptr<glm::mat4[]> (new glm::mat4[m_Count]);
     for (int i = 0; i < m_Count ; i++)
    {
        Transform t;
        PointInCircle(t, radius);
        ret[i] = t.ToMatrix4();
    }
    return ret;
}


void PointsGenerator::PointInSquare(Transform &t, float radius)
{
    float x = util::RandInRange(-radius, radius);
    float z = util::RandInRange(-radius, radius);
    float y = 0.0f;
    t.position = glm::vec3(x, y, z);
    AssignRS(t);
}

std::shared_ptr<Transform[]>  PointsGenerator::Square(float radius)
{
    std::shared_ptr<Transform[]> ret = std::shared_ptr<Transform[]> (new Transform[m_Count]);
    for (int i = 0; i < m_Count ; i++)
    {
        PointInSquare(ret[i], radius);
    }
    return ret;
}

std::shared_ptr<glm::mat4[]>  PointsGenerator::SquareMat(float radius)
{
    std::shared_ptr<glm::mat4[]> ret = std::shared_ptr<glm::mat4[]> (new glm::mat4[m_Count]);
    for (int i = 0; i < m_Count ; i++)
    {
        Transform t;
        PointInSquare(t, radius);
        ret[i] = t.ToMatrix4();
    }

    return ret;
}

std::shared_ptr<glm::mat4[]>  PointsGenerator::GridMat(int rows, int columns, float interval, glm::vec3 up, glm::vec3 forward, glm::vec3 offset)
{
   // glm::vec3 nDirection = glm::normalize(direction);
    m_Count = rows * columns;
    std::shared_ptr<glm::mat4[]> ret = std::shared_ptr<glm::mat4[]> (new glm::mat4[m_Count]);
    int index = 0;
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < columns; j++)
        {
            Transform t;
            //PointInGrid(t, );
            float x =  interval * i;
            float y =  interval * j;
            glm::mat4 mat = glm::mat4(1.0);
            //mat = glm::translate(mat, offset);
            //mat = glm::rotate(mat, nDirection, {0, 1, 0});
            //glm::vec3 dest = nDirection * interval * j
            glm::vec3 _offset = offset + glm::vec3(x, y, 0);
            //PointInGrid(t, _offset);
            t.position = _offset;
            t.rotate = glm::vec3(0, 1, 0);
            t.scale = glm::vec3(1.0f);
            t.angle = 0.0f;
            ret[index] = t.ToMatrix4();
            index += 1;
        }
    }

    return ret;
}



void PointsGenerator::AssignRS(Transform &t)
{
    t.rotate = m_RotationVariant;
    t.scale  = glm::vec3(util::RandInRange(1.0 - m_ScaleVariant, 1 + m_ScaleVariant));
    t.angle  = util::RandInRange(0.0f, 1.0f) * (float)(M_PI*2);
}
void PointsGenerator::SetScaleVariant(float scale)
{
    m_ScaleVariant = scale;
}