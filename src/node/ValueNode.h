#pragma once
#include <glm/glm.hpp>
#include "../textures/Texture.h"
#include <string>
#include <any>
#include <map>
#include "ValueSlot.h"
#include <unordered_map>
#include <utility>
#include <vector>

enum class ValueNodeType
{
    RAW, FLOAT, VECTOR, TEXTURE, MATERIAL, STRING, POINTER, TUPLE, PAIR, NONE
};

class ValueNodeValue
{
private:
    std::any m_Value;
    ValueNodeType m_Type;
public:
    ValueNodeValue();
    ValueNodeValue(float);
    ValueNodeValue(glm::vec3);
    ValueNodeValue(void *);
    ValueNodeValue(std::string);
    ValueNodeValue(ValueNodeType type, std::any value);
    ValueNodeValue(const ValueNodeValue &v);

    glm::vec3 GetVector();
    float GetFloat();
    void *GetPtr();
    std::string GetStr();
    ValueNodeType GetType();
    std::pair<glm::vec3, void *> GetPair();
};

class ValueNode 
{
protected:
    unsigned int m_InputNum = 0;
    unsigned int m_OutputNum = 0;
    std::unordered_map<unsigned int, ValueNodeValue> m_Intrinsic;
    std::vector<ValueSlotInput> m_Inputs;
    std::vector<ValueNodeValue> m_Outputs;
    ValueNodeType type;
public:
    ValueNode();
    ~ValueNode();
    virtual void InitInputs() {};
    virtual void InitOutputs() {};
    virtual void Calc();
    virtual ValueNodeType GetType();
    void UpdateInputValue(unsigned int key, ValueNode* newValueNode);
    ValueNodeValue GetInputValueByKey(unsigned int);
    ValueNodeValue GetFirstOutputValue();
    ValueNode * GetInputValueNodeByKey(unsigned int key);
};