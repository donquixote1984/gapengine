#include "Transform.h"

Transform::Transform(){}
Transform::Transform(glm::vec3 pos)
{
    position = pos;
}

glm::mat4 Transform::ToMatrix4()
{
    glm::mat4 mat = glm::mat4(1);
    mat = glm::translate(mat, position);
    mat = glm::rotate(mat, angle, rotate);
    mat = glm::scale(mat, scale);
    return mat;
}

glm::mat4 Transform::ToMatrix4(float scaleVariant)
{
    glm::mat4 mat= glm::mat4(1);
    scale  = glm::vec3(util::RandInRange(1.0 - scaleVariant, 1 + scaleVariant));
    angle = util::RandInRange(0.0f, 1.0f) * (float)(M_PI*2);
    rotate = glm::vec3(.0, 1.0, 0.0);
    mat = glm::translate(mat, position);
    mat = glm::rotate(mat, angle, rotate);
    mat = glm::scale(mat, scale);
    return mat;
}

glm::mat4 Transform::ToMatrix4(float scaleVariant, glm::vec3 r)
{
    glm::mat4 mat=glm::mat4(1);
    //float randRot = util::RandInRange(0.0f, M_2_PI);

    scale  = glm::vec3(util::RandInRange(1.0 - scaleVariant, 1 + scaleVariant));
    angle = util::RandInRange(0.0f, 1.0f) * (float)(M_PI*2);
    mat = glm::translate(mat, position);
    mat = glm::rotate(mat, angle, r);
    mat = glm::scale(mat, scale);
    return mat;
}

