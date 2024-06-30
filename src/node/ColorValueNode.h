#pragma once
#include <glm/glm.hpp>
#include <tuple>
#include <utility>
#include "VectorValueNode.h"
#include "FloatValueNode.h"
#include "TextureNode.h"
#include <any>

class ColorValueNode: public ValueNode
{
public:
    ColorValueNode();
    ColorValueNode(glm::vec3 &);
    ColorValueNode(float x, float y, float z);
    ColorValueNode(float x, float y, float z, const std::string &filePath);
    ColorValueNode(const std::string &filePath);

    void AttachColor(float x, float y, float z);
    void AttachTexture(const std::string & filePath);

    void InitInputs() override;
    void InitOutputs() override;
    void Calc() override;
    ValueNodeType GetType() override;
};