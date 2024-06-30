#pragma once
#include <any>
#include <string>
#include "ValueNode.h"

class ValueNode;
enum ValueSlotType {
    ///INPUT,
    //VALUE
//    FILE,
    COLOR,
    VECTOR,
    FLOAT,
    TEXTURE,
    STRING
};

struct ValueSlotInput
{
    ValueSlotType type;
    unsigned int key;
    std::string name;
    ValueNode *valueNode;
    // value;
};

struct ValueSlotOutput {
    unsigned int key;
    std::string name;
    // value;
};