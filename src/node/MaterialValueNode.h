#pragma once
#include "ValueNode.h"
#include "ColorValueNode.h"
#include "FloatValueNode.h"
#include "TextureNode.h"
#include "../render/DefaultMaterial.h"
#include <tuple>
#include <any>
#include <type_traits>
#include "MaterialTemplate.h"
#include <glm/glm.hpp>
#include "MaterialTemplate.h"


class MaterialTemplate;
class MaterialValueNode: public ValueNode
{
protected:
    MaterialTemplate * m_Template;
public:
    MaterialValueNode(MaterialTemplate * );
    void InitInputs() override;
    void InitOutputs() override;
    void Calc() override;
    void MakeMaterialAttrib(MaterialAttrib & mAttrib, ValueNodeValue value);
    ValueNodeType GetType() override;
    template <typename T, typename V>
    T * GetOrCreateMaterial(V v)
    {
         T * material;
        if ((m_Outputs[0].GetPtr()) != nullptr)
        {
            material = (T*)m_Outputs[0].GetPtr();
        } else {
            material = new T(v);
            ValueNodeValue vnv = (void *) material;
            m_Outputs[0] = vnv;
        }
        return material;
    }

    template <typename T>
    T * GetOrCreateMaterial()
    {
        T * material;
        if ((m_Outputs[0].GetPtr()) != nullptr)
        {
            material = (T*)m_Outputs[0].GetPtr();
        } else {
            material = new T();
            ValueNodeValue vnv = (void *) material;
            m_Outputs[0] = vnv;
        }
        return material;
    }
};